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
	m_pService = Service::create();
	m_pListTypes = TypeList::create();
	m_pListElements = RequestResponseElementList::create();
	m_pListMessages = MessageList::create();
	m_pListOperations = OperationList::create();
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

	if(m_szCurrentSection == "") {
		if(qName == "wsdl:definitions") {
			if(attributes.index("name") != -1) {
				m_pService->setName(attributes.value("name"));
			}
		}

		if(qName == "xs:schema") {
			QString szTargetNamespace;

			if(attributes.index("targetNamespace") != -1) {
				szTargetNamespace = attributes.value("targetNamespace");
			}

			for(int i=0; i < attributes.length(); ++i) {
				if(attributes.value(i) == szTargetNamespace && attributes.qName(i) != "targetNamespace") {
					m_szTargetNamespacePrefix = attributes.localName(i);
					qDebug("[QWSDLParserHandler::startElement] Target namespace prefix found: %s", qPrintable(m_szTargetNamespacePrefix));
				}
			}
		}

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
					reader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
					reader.setFeature("http://xml.org/sax/features/namespaces", true);
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

				if(!m_pListTypes->getByName(szName, m_szTargetNamespacePrefix).isNull()) {
					m_pCurrentType = m_pListTypes->getByName(szName, m_szTargetNamespacePrefix);
					if(m_pCurrentType->getClassType() == Type::TypeUnknown) {

						m_pCurrentType = SimpleType::create();
						m_pCurrentType->setLocalName(szName);
						m_pCurrentType->setNamespace(m_szTargetNamespacePrefix);
					}

				}else{
					m_pCurrentType = SimpleType::create();
					m_pCurrentType->setLocalName(szName);
					m_pCurrentType->setNamespace(m_szTargetNamespacePrefix);
				}
			}
		}

		if(qName == "xs:complexType") {
			m_szCurrentSection = qName;

			iRes = attributes.index("name");
			if(iRes != -1) {
				QString szName = attributes.value("name");

				if(!m_pListTypes->getByName(szName, m_szTargetNamespacePrefix).isNull()) {
					m_pCurrentType = m_pListTypes->getByName(szName, m_szTargetNamespacePrefix);
					if(m_pCurrentType->getClassType() == Type::TypeUnknown) {

						m_pCurrentType = ComplexType::create();
						m_pCurrentType->setLocalName(szName);
						m_pCurrentType->setNamespace(m_szTargetNamespacePrefix);
					}

				}else{
					m_pCurrentType = ComplexType::create();
					m_pCurrentType->setLocalName(szName);
					m_pCurrentType->setNamespace(m_szTargetNamespacePrefix);
				}
			}
		}

		if(qName == "xs:element" &&
				attributes.index("name") != -1 &&
				attributes.count() == 1) {
			//We are in Message Request/Response elements list section
			m_szCurrentSection = qName;

			m_pCurrentElement = RequestResponseElement::create();
			m_pCurrentElement->setLocalName(attributes.value("name"));
			m_pCurrentElement->setNamespace(m_szTargetNamespacePrefix);
		}

		if(qName == "wsdl:message") {
			m_szCurrentSection = qName;

			m_pCurrentMessage = Message::create();
			if(attributes.index("name") != -1) {
				m_pCurrentMessage->setLocalName(attributes.value("name"));
				m_pCurrentMessage->setNamespace(m_szTargetNamespacePrefix);
			}
		}

		if(qName == "wsdl:operation") {
			m_szCurrentSection = qName;

			if(attributes.index("name") != -1) {
				QString szName = attributes.value("name");

				OperationSharedPtr pOperation = m_pListOperations->getByName(szName);
				if(pOperation.isNull()) {
					m_pCurrentOperation = Operation::create();
					m_pCurrentOperation->setName(szName);
				}
			}
		}
	}


	if(m_szCurrentSection == "xs:simpleType" && !m_pCurrentType.isNull()) {
		if(qName == "xs:restriction") {
			iRes = attributes.index("base");
			if(iRes != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentType);
				pSimpleType->setVariableTypeFromString(attributes.value("base"));
				pSimpleType->setRestricted(true);
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
		if(qName == "xs:enumeration") {
			if(attributes.index("value") != -1) {

				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentType);
				QString szVal = attributes.value("value");
				pSimpleType->addEnumerationValue(szVal);
			}

		}
	}


	if( (m_szCurrentSection == "xs:complexType" || m_szCurrentType == "xs:complexType") && !m_pCurrentType.isNull()) {
		if(qName == "xs:extension") {

			iRes = attributes.index("base");
			if(iRes != -1) {
				QString szName = attributes.value("base");
				if(!szName.startsWith("xs:")) {
					TypeSharedPtr pType = m_pListTypes->getByName(szName.split(":")[1], szName.split(":")[0]);
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
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];
				TypeSharedPtr pType = m_pListTypes->getByName(szNamespace, szLocalName);
				if(!pType.isNull()){
					attr->setType(pType);
				}else{
					qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith("xs:")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(szValue);
						pType->setName(attr->getName());
						attr->setType(pType);
					}else{
						TypeSharedPtr pType = Type::create();
						pType->setLocalName(szLocalName);
						pType->setNamespace(szNamespace);
						m_pListTypes->append(pType);
						attr->setType(pType);
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
			if(iRes != -1) {
				element->setName(attributes.value("name"));
			}

			iRes = attributes.index("type");
			if(iRes != -1) {
				QString szValue = attributes.value("type");
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];
				TypeSharedPtr pType = m_pListTypes->getByName(szNamespace, szLocalName);
				if(!pType.isNull()){
					element->setType(pType);
				}else{
					qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith("xs:")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(szValue);
						pType->setName(element->getName());
						element->setType(pType);
					}else{
						TypeSharedPtr pType = Type::create();
						pType->setLocalName(szLocalName);
						pType->setNamespace(szNamespace);
						m_pListTypes->append(pType);
						element->setType(pType);
					}
				}
			}


			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pCurrentType);
			pComplexType->addElement(element);
		}
	}

	if(m_szCurrentSection == "xs:element" && !m_pCurrentElement.isNull()) {
		if(qName == "xs:complexType") {
			m_szCurrentType = qName;
			m_pCurrentType = ComplexType::create();
		}
	}

	if(m_szCurrentSection == "wsdl:message" && !m_pCurrentMessage.isNull()) {
		if(qName == "wsdl:part") {
			if(attributes.index("name") != -1 && attributes.index("element") != -1) {
				if(attributes.value("name") == "parameters") {
					QString qualifiedName = attributes.value("element");

					if(qualifiedName.contains(":")) {
						RequestResponseElementSharedPtr pElement;
						pElement = m_pListElements->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
						m_pCurrentMessage->setParameter(pElement);
					}
				}
			}
		}
	}

	if(m_szCurrentSection == "wsdl:operation" && !m_pCurrentOperation.isNull()) {
		if(qName == "wsdl:input") {
			if(attributes.index("message") != -1) {
				QString qualifiedName = attributes.value("message");

				if(qualifiedName.contains(":")) {
					MessageSharedPtr pMessage;
					pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
					m_pCurrentOperation->setInputMessage(pMessage);
				}
			}
		}
		if(qName == "wsdl:output") {
			if(attributes.index("message") != -1) {
				QString qualifiedName = attributes.value("message");

				if(qualifiedName.contains(":")) {
					MessageSharedPtr pMessage;
					pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
					m_pCurrentOperation->setOutputMessage(pMessage);
				}
			}
		}
	}

	return true;
}

bool
QWSDLParserHandler::endElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName)
{
	TypeSharedPtr pType;
	TypeListSharedPtr pListTypes;
	TypeList::const_iterator type;

	RequestResponseElementList::const_iterator requestResponseElement;
	ElementList::const_iterator element;
	AttributeList::const_iterator attr;


	if(qName == "xs:simpleType" && !m_pCurrentType.isNull()) {
		m_szCurrentSection = "";
		m_pListTypes->add(m_pCurrentType);
		m_pCurrentType.clear();

	}

	if(qName == "xs:complexType" && !m_pCurrentType.isNull()) {
		if(m_szCurrentSection == "xs:element" && !m_pCurrentElement.isNull()) {
			m_szCurrentType = "";
			m_pCurrentElement->setComplexType(qSharedPointerCast<ComplexType>(m_pCurrentType));
			m_pCurrentType.clear();
		}else{
			m_szCurrentSection = "";
			m_pListTypes->add(m_pCurrentType);
			m_pCurrentType.clear();
		}

	}

	if(qName == "xs:element" && !m_pCurrentElement.isNull() && m_szCurrentType != "xs:complexType") {
		m_szCurrentSection = "";
		m_pListElements->append(m_pCurrentElement);
		m_pCurrentElement.clear();

	}

	if(qName == "wsdl:message") {
		m_szCurrentSection = "";
		m_pListMessages->append(m_pCurrentMessage);
		m_pCurrentMessage.clear();

	}

	if(qName == "wsdl:operation" && !m_pCurrentOperation.isNull()) {
		m_szCurrentSection = "";
		m_pListOperations->append(m_pCurrentOperation);
		m_pService->addOperation(m_pCurrentOperation);
		m_pCurrentOperation.clear();

	}

	if(qName == "wsdl:types") {

		for(type = m_pListTypes->constBegin(); type != m_pListTypes->constEnd(); ++type) {

			if( (*type)->getClassType() == Type::TypeComplex) {

				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(*type);
				if(pComplexType->getExtensionType()) {
					if(pComplexType->getExtensionType()->getClassType() == Type::TypeUnknown) {
						pType = m_pListTypes->getByName(
								pComplexType->getExtensionType()->getLocalName(),
								pComplexType->getExtensionType()->getNamespace());
						pComplexType->setExtensionType(pType);
					}
				}
				if(pComplexType->getElementList()->count() > 0) {
					//Parcours elment list

					for(element = pComplexType->getElementList()->constBegin();
							element != pComplexType->getElementList()->constEnd(); ++element) {

						if( (*element)->getType()->getClassType() == Type::TypeUnknown) {
							pType = m_pListTypes->getByName((*element)->getType()->getLocalName(), (*element)->getType()->getNamespace());
							(*element)->setType(pType);
						}
					}

				}
				if(pComplexType->getAttributeList()->count() > 0) {
					//Parcours attr list

					for(attr = pComplexType->getAttributeList()->constBegin();
							attr != pComplexType->getAttributeList()->constEnd(); ++attr) {

						if( (*attr)->getType()->getClassType() == Type::TypeUnknown) {
							pType = m_pListTypes->getByName((*attr)->getType()->getLocalName(), (*attr)->getType()->getNamespace());
							(*attr)->setType(pType);
						}


					}
				}
			}
		}

		for(requestResponseElement = m_pListElements->constBegin();
				requestResponseElement != m_pListElements->constEnd(); ++requestResponseElement) {

			ComplexTypeSharedPtr pComplexType = (*requestResponseElement)->getComplexType();
			if(pComplexType->getExtensionType()) {
				if(pComplexType->getExtensionType()->getClassType() == Type::TypeUnknown) {
					pType = m_pListTypes->getByName(
							pComplexType->getExtensionType()->getLocalName(),
							pComplexType->getExtensionType()->getNamespace());
					pComplexType->setExtensionType(pType);
				}
			}
			if(pComplexType->getElementList()->count() > 0) {
				//Parcours elment list

				for(element = pComplexType->getElementList()->constBegin();
						element != pComplexType->getElementList()->constEnd(); ++element) {

					if( (*element)->getType()->getClassType() == Type::TypeUnknown) {
						pType = m_pListTypes->getByName((*element)->getType()->getLocalName(),
								(*element)->getType()->getNamespace());
						(*element)->setType(pType);
					}
				}

			}
			if(pComplexType->getAttributeList()->count() > 0) {
				//Parcours attr list

				for(attr = pComplexType->getAttributeList()->constBegin();
						attr != pComplexType->getAttributeList()->constEnd(); ++attr) {

					if( (*attr)->getType()->getClassType() == Type::TypeUnknown) {
						pType = m_pListTypes->getByName((*attr)->getType()->getLocalName(),
								(*attr)->getType()->getNamespace());
						(*attr)->setType(pType);
					}


				}
			}
		}
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

RequestResponseElementListSharedPtr QWSDLParserHandler::getElementList() const
{
	return m_pListElements;
}

ServiceSharedPtr QWSDLParserHandler::getService() const
{
	return m_pService;
}
