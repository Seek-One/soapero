/*
 * QWSDLParserHandler.cpp
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#include <QBuffer>

#include "Model/ComplexType.h"
#include "Model/SimpleType.h"
#include "Model/Type.h"

#include "QWSDLParserHandler.h"

QWSDLParserHandler::QWSDLParserHandler()
{
	m_pListTypes = TypeList::create();
}

QWSDLParserHandler::~QWSDLParserHandler()
{

}

bool QWSDLParserHandler::startElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName,
		const QXmlAttributes &attributes)
{
	int iRes;

	if(qName == "xs:import") {
		iRes = attributes.index("schemaLocation");
		if(iRes != -1) {

			QString szLocation(attributes.value("schemaLocation"));
			QFile file(szLocation);

			if(file.open(QFile::ReadOnly)) {

				QByteArray bytes = file.readAll();
				QBuffer buffer;
				buffer.setData(bytes);

				QXmlInputSource source(&buffer);
				QXmlSimpleReader reader;
				QWSDLParserHandler handler;
				reader.setContentHandler(&handler);
				reader.setErrorHandler(&handler);
				if(reader.parse(source)){
					TypeListSharedPtr pList = handler.getTypeList();
					TypeList::const_iterator type;
					for(type = pList->constBegin(); type != pList->constEnd(); ++type) {
						m_pListTypes->add(*type);
					}
				}else{
					qWarning("[QWSDLParserHandler::startElement] Error to parse file %s (error: %s)",
							qPrintable(szLocation),
							qPrintable(reader.errorHandler()->errorString()));
				}
			}else{
				qWarning("[QWSDLParserHandler::startElement] Error for opening file %s (error: %s)",
						qPrintable(szLocation),
						qPrintable(file.errorString()));
			}
		}
	}



	if(qName == "xs:simpleType") {
		m_szCurrentSection = qName;

		iRes = attributes.index("name");
		if(iRes != -1) {
			QString szName = attributes.value("name");

			if(!m_pListTypes->getByName(szName).isNull()) {
				m_pCurrentType = m_pListTypes->getByName(szName);
			}else{
				m_pCurrentType = SimpleType::create();
			}

			m_pCurrentType->setName(szName);
		}
	}

	if(qName == "xs:complexType") {
		m_szCurrentSection = qName;

		iRes = attributes.index("name");
		if(iRes != -1) {
			QString szName = attributes.value("name");

			if(!m_pListTypes->getByName(szName).isNull()) {
				m_pCurrentType = m_pListTypes->getByName(szName);
			}else{
				m_pCurrentType = ComplexType::create();
			}

			m_pCurrentType->setName(szName);
		}
	}



	if(m_szCurrentSection == "xs:simpleType" && !m_pCurrentType.isNull()) {
		if(qName == "xs:restriction") {
			iRes = attributes.index("base");
			if(iRes != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentType);
				pSimpleType->setVariableTypeFromString(attributes.value("base"));
			}

		}
		if(qName == "xs:maxLength") {
			iRes = attributes.index("value");
			if(iRes != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentType);
				pSimpleType->setMaxLength(attributes.value("value").toUInt());
			}

		}
		if(qName == "xs:minLength") {
			iRes = attributes.index("value");
			if(iRes != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentType);
				pSimpleType->setMinLength(attributes.value("value").toUInt());
			}

		}
	}


	if(m_szCurrentSection == "xs:complexType" && !m_pCurrentType.isNull()) {
		if(qName == "xs:extension") {

			iRes = attributes.index("base");
			if(iRes != -1) {
				QString szName = attributes.value("base");
				if(!szName.startsWith("xs:")) {
					TypeSharedPtr pType = m_pListTypes->getByName(szName.split(":")[1]);
					qSharedPointerCast<ComplexType>(m_pCurrentType)->setExtensionType(pType);
				}
			}

		}

		if(qName == "xs:attribute") {
			AttributeSharedPtr attr = Attribute::create();

			iRes = attributes.index("name");
			if(iRes != -1) {
				attr->setName(attributes.value("name"));
			}

			iRes = attributes.index("type");
			if(iRes != -1) {
				QString szValue = attributes.value("type");
				if(szValue.startsWith("pt:")) {
					TypeSharedPtr pType = m_pListTypes->getByName(szValue.split(":")[1]);
					attr->setType(pType);
				}else if(szValue.startsWith("xs:")) {
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(szValue);
					pType->setName(attr->getName());
					attr->setType(pType);
				}else{
					if(szValue.contains(":")) {
						TypeSharedPtr pType = m_pListTypes->getByName(szValue.split(":")[1]);

						if(!pType.isNull()){
							attr->setType(pType);
						}else{
							qWarning("[QWSDLParserHandler::endElement] Type %s is not found, we create it", qPrintable(szValue));
							ComplexTypeSharedPtr pComplexType = ComplexType::create();
							pComplexType->setName(szValue.split(":")[1]);
							m_pListTypes->append(pComplexType);
							attr->setType(pComplexType);
						}
					}
				}
			}

			iRes = attributes.index("use");
			if(iRes != -1) {
				QString szValue = attributes.value("use");
				if(szValue == "required") {
					attr->setRequired(true);
				}
			}

			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pCurrentType);
			pComplexType->addAttribute(attr);
		}
		if(qName == "xs:element") {
			ElementSharedPtr element = Element::create();

			iRes = attributes.index("name");
			qDebug(qPrintable(attributes.value("name")));
			if(iRes != -1) {
				element->setName(attributes.value("name"));
				if(attributes.value("name") == "tdc:DoorFaultState") {
					qDebug("TEST");
				}
			}

			iRes = attributes.index("type");
			if(iRes != -1) {
				QString szValue = attributes.value("type");
				if(szValue.startsWith("pt:")) {
					TypeSharedPtr pType = m_pListTypes->getByName(szValue.split(":")[1]);
					element->setType(pType);
				}else if(szValue.startsWith("xs:")) {
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(szValue);
					pType->setName(element->getName());
					element->setType(pType);
				}else{
					if(szValue.contains(":")) {
						TypeSharedPtr pType = m_pListTypes->getByName(szValue.split(":")[1]);

						if(!pType.isNull()){
							element->setType(pType);
						}else{
							qWarning("[QWSDLParserHandler::endElement] Type %s is not found, we create it", qPrintable(szValue));
							ComplexTypeSharedPtr pComplexType = ComplexType::create();
							pComplexType->setName(szValue.split(":")[1]);
							m_pListTypes->append(pComplexType);
							element->setType(pComplexType);
						}
					}
				}
			}

			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pCurrentType);
			pComplexType->addElement(element);
		}
	}

	return true;
}

bool
QWSDLParserHandler::endElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName)
{

	if(qName == "xs:simpleType" && !m_pCurrentType.isNull()) {
		m_szCurrentSection = "";
		m_pListTypes->add(m_pCurrentType);
		m_pCurrentType.clear();

	}

	if(qName == "xs:complexType" && !m_pCurrentType.isNull()) {
		m_szCurrentSection = "";
		m_pListTypes->add(m_pCurrentType);
		m_pCurrentType.clear();

	}

	return true;
}

bool
QWSDLParserHandler::characters(const QString &str)
{
	m_szCurrentText = str.trimmed();
	return true;
}

bool
QWSDLParserHandler::fatalError(const QXmlParseException &exception)
{
	qWarning("[QWSDLParserHandler] Error while parsing XML content : (line %d) %s", exception.lineNumber(), qPrintable(exception.message()));
	return false;
}

TypeListSharedPtr QWSDLParserHandler::getTypeList() const
{
	return m_pListTypes;
}
