/*
 * QWSDLParser.cpp
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#include <QFile>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include "Model/ComplexType.h"
#include "Model/SimpleType.h"
#include "Model/Type.h"

#include "WSDLAttributes.h"

#include "QWSDLParser.h"

QWSDLParser::QWSDLParser()
{
	m_bWaitForSoapEnvelopeFault = false;

	m_pDataLoaded = QSharedPointer<QWSDLData>(new QWSDLData());

	m_pService = Service::create();
	m_pListTypes = TypeList::create();
	m_pListRequestResponseElements = RequestResponseElementList::create();
	m_pListMessages = MessageList::create();
	m_pListOperations = OperationList::create();
	m_pListAttributes = AttributeList::create();
	m_pListElements = ElementList::create();

	initXMLAttributes();

	m_iLogIndent = 0;
}

QWSDLParser::~QWSDLParser()
{

}

void QWSDLParser::initXMLAttributes()		// https://www.w3.org/2001/xml.xsd
{
	// https://www.w3.org/2001/xml.xsd#att_lang
	AttributeSharedPtr pAttribute = Attribute::create();
	pAttribute->setNamespace("xml");
	pAttribute->setName("lang");
	SimpleTypeSharedPtr pSimpleType = SimpleType::create();
	pSimpleType->setVariableTypeFromString("xs", "xs:string");
	pSimpleType->setName("lang");
	m_pListTypes->append(pSimpleType);
	pAttribute->setType(pSimpleType);
	m_pListAttributes->append(pAttribute);
}

void QWSDLParser::setLogIndent(int iIdent)
{
	m_iLogIndent = iIdent;
}

void QWSDLParser::setWSDLData(const QSharedPointer<QWSDLData>& pDataLoaded)
{
	m_pDataLoaded = pDataLoaded;
}

void QWSDLParser::setInitialNamespaceDeclarationList(const QWSDLNamespaceDeclarations& listNamespaceDeclarations)
{
	m_listNamespaceDeclarations = listNamespaceDeclarations;
}

const QWSDLNamespaceDeclarations& QWSDLParser::getInitialNamespaceDeclarationList() const
{
	return m_listNamespaceDeclarations;
}

void QWSDLParser::setInitialNamespaceUri(const QString& szNamespaceUri)
{
	pushCurrentTargetNamespace(szNamespaceUri);
}

bool QWSDLParser::parse(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamReader::TokenType iTokenType;

	while (!xmlReader.atEnd() && bRes)
	{
		iTokenType = xmlReader.readNext();

		if (iTokenType == QXmlStreamReader::StartElement)
		{
			QString szTagName =  xmlReader.name().toString();

			logParser("processing: " + szTagName);
			incrLogIndent();

			// WSDL files
			if (szTagName == TAG_DEFINITIONS) {
				bRes = readDefinitions(xmlReader);
			}
			// XSD files
			if(szTagName == TAG_SCHEMA){
				bRes = readSchema(xmlReader);
			}

			decrLogIndent();
		}

		if(iTokenType == QXmlStreamReader::EndDocument)
		{
			endDocument();
		}
	}

	if (xmlReader.hasError()) {
		qCritical("[XML] Event list parse error: %s", qPrintable(xmlReader.errorString()));
		return false;
	}

	if (!bRes) {
		qCritical("[XML] Invalid event item");
		return false;
	}

	return true;
}

bool QWSDLParser::endDocument()
{
	TypeSharedPtr pType;
	TypeList::const_iterator type;
	TypeListSharedPtr pListTypesToRemove = TypeList::create();;

	RequestResponseElementList::iterator requestResponseElement;

	ElementList::iterator element;
	ElementSharedPtr pElement;

	AttributeList::iterator attr;
	AttributeSharedPtr pAttribute;

	OperationList::iterator operation;

	// TODO: resolve ref for attributes

	// Resolve ref for element
	for(element = m_pListElements->begin(); element != m_pListElements->end(); ++element){
		if((*element)->needRef()){
			ElementSharedPtr pRefElement = getElementByRef((*element)->getRefValue());
			if(pRefElement){
				(*element)->setRef(pRefElement);
			}
		}
	}

	// Resolve types
	for(type = m_pListTypes->constBegin(); type != m_pListTypes->constEnd(); ++type) {
		if((*type)->getClassType() == Type::TypeComplex) {

			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(*type);
			if(pComplexType->getExtensionType()) {
				if(pComplexType->getExtensionType()->getClassType() == Type::TypeUnknown) {
					pType = getTypeByName(pComplexType->getExtensionType()->getLocalName(), pComplexType->getExtensionType()->getNamespace());
					pComplexType->setExtensionType(pType);
				}
			}
			if(pComplexType->getElementList()->count() > 0) {
				//Parcours elment list

				for(element = pComplexType->getElementList()->begin();
				element != pComplexType->getElementList()->end(); ++element) {

					if((*element)->hasRef()){
						pElement = (*element)->getRef();
					}else{
						pElement = *element;
					}

					// Resolve element type
					if(pElement->getType() && pElement->getType()->getClassType() == Type::TypeUnknown) {
						pType = getTypeByName(pElement->getType()->getLocalName(), pElement->getType()->getNamespace(), pListTypesToRemove);
						while(pType && pType->getClassType() == Type::TypeUnknown) {
							pListTypesToRemove->append(pType);
							pType = getTypeByName(pElement->getType()->getLocalName(), pElement->getType()->getNamespace(), pListTypesToRemove);
						}
						pElement->setType(pType);
					}
				}

			}
			if(pComplexType->getAttributeList()->count() > 0){
				//Parcours attr list

				for(attr = pComplexType->getAttributeList()->begin();
				attr != pComplexType->getAttributeList()->end(); ++attr){
					if((*attr)->hasRef()){
						pAttribute = (*attr)->getRef();
					}else{
						pAttribute = *attr;
					}

					if(pAttribute->getType() && pAttribute->getType()->getClassType() == Type::TypeUnknown){
						pType = getTypeByName(pAttribute->getType()->getLocalName(), pAttribute->getType()->getNamespace());
						pAttribute->setType(pType);
					}
				}
			}
		}
	}

	for(type = pListTypesToRemove->constBegin(); type != pListTypesToRemove->constEnd(); ++type){
		m_pListTypes->removeAll(*type);
	}
	pListTypesToRemove->clear();

	for(requestResponseElement = m_pListRequestResponseElements->begin();
	requestResponseElement != m_pListRequestResponseElements->end(); ++requestResponseElement) {

		ComplexTypeSharedPtr pComplexType = (*requestResponseElement)->getComplexType();
		if(pComplexType->getExtensionType()) {
			if(pComplexType->getExtensionType()->getClassType() == Type::TypeUnknown) {
				pType = getTypeByName(
						pComplexType->getExtensionType()->getLocalName(),
						pComplexType->getExtensionType()->getNamespace());
				pComplexType->setExtensionType(pType);
			}
		}
		if(pComplexType->getElementList()->count() > 0) {
			//Parcours elment list

			for(element = pComplexType->getElementList()->begin();
				element != pComplexType->getElementList()->end(); ++element)
			{

				if((*element)->hasRef()){
					pElement = (*element)->getRef();
				}else{
					pElement = *element;
				}

				if(pElement->getType() && pElement->getType()->getClassType() == Type::TypeUnknown) {
					pType = getTypeByName(pElement->getType()->getLocalName(),
										  pElement->getType()->getNamespace());
					pElement->setType(pType);
				}
			}
		}
		if(pComplexType->getAttributeList()->count() > 0) {
			//Parcours attr list

			for(attr = pComplexType->getAttributeList()->begin();
			attr != pComplexType->getAttributeList()->end(); ++attr) {

				if( (*attr)->getType()->getClassType() == Type::TypeUnknown) {
					pType = getTypeByName((*attr)->getType()->getLocalName(),
										  (*attr)->getType()->getNamespace());
					(*attr)->setType(pType);
				}


			}
		}
	}

	// Resolve recursive inclusion (class1 includes class2 and class2 includes class1)
	// by using pointers (only check elements type for now).
	// TODO: check attributes type too.
	for(type = m_pListTypes->constBegin(); type != m_pListTypes->constEnd(); ++type){
		if((*type)->getClassType() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(*type);

			// Add soap envelope default fault type if exists
			if(pComplexType->isSoapEnvelopeFault()){
				for(operation = m_pListOperations->begin(); operation != m_pListOperations->end(); ++operation){
					(*operation)->setSoapEnvelopeFaultType(pComplexType);
				}
			}

			if(pComplexType->getElementList() && pComplexType->getElementList()->count() > 0){
				for(element = pComplexType->getElementList()->begin(); element != pComplexType->getElementList()->end(); ++element){
					if((*element)->hasRef()){
						pElement = (*element)->getRef();
					}else{
						pElement = *element;
					}

					if(pElement->getType() && pElement->getType()->getClassType() == Type::TypeComplex){
						ComplexTypeSharedPtr pComplexTypeElem = qSharedPointerCast<ComplexType>(pElement->getType());

						if(pComplexTypeElem->getElementList() && pComplexTypeElem->getElementList()->count() > 0){
							ElementList::const_iterator iter;
							for(iter = pComplexTypeElem->getElementList()->constBegin(); iter != pComplexTypeElem->getElementList()->constEnd(); ++iter){
								if((*iter)->getType() && (*iter)->getType()->getNameWithNamespace() == pComplexType->getNameWithNamespace()){
									// No need to use a pointer if it's a list, or if it's already nested
									if(((*iter)->getMaxOccurs() == 1) && !(*iter)->isNested()){
										(*iter)->setIsPointer(true);
										pElement->setIsPointer(true);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}


bool QWSDLParser::readXMLNamespaces(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamNamespaceDeclarations decls = xmlReader.namespaceDeclarations();
	QXmlStreamNamespaceDeclarations::const_iterator iter;

	QString szAttrLocalName;
	QString szAttrValue;

	for(iter = decls.constBegin(); iter != decls.constEnd(); ++iter)
	{
		const QXmlStreamNamespaceDeclaration& decl = (*iter);

		szAttrLocalName = decl.prefix().toString();
		szAttrValue = decl.namespaceUri().toString();

		m_listNamespaceDeclarations.insert(szAttrLocalName, szAttrValue);
	}

	return bRes;
}

bool QWSDLParser::readDefinitions(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	// Read XML namespaces
	bRes = readXMLNamespaces(xmlReader);

	// Parse attributes
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();
	QString szTargetNamespace;
	if(bRes){
		if(xmlAttrs.hasAttribute(ATTR_NAME))
		{
			m_pService->setName(xmlAttrs.value(ATTR_NAME).toString());
		}
		if(xmlAttrs.hasAttribute(ATTR_TARGET_NAMESPACE))
		{
			szTargetNamespace = xmlAttrs.value(ATTR_TARGET_NAMESPACE).toString();
			m_pService->setTargetNamespace(szTargetNamespace);
		}
	}

	// Enter target namespace if any
	if(!szTargetNamespace.isEmpty()){
		bRes = pushCurrentTargetNamespace(szTargetNamespace);
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		logParser("processing: " + xmlReader.name().toString());

		if (xmlReader.name() == TAG_TYPES) {
			bRes = readTypes(xmlReader);
		}else if (xmlReader.name() == TAG_MESSAGE) {
			bRes = readMessage(xmlReader);
		}else if (xmlReader.name() == TAG_PORTTYPE) {
			bRes = readPortType(xmlReader);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// Exit target namespace if any
	if(!szTargetNamespace.isEmpty()){
		popCurrentTargetNamespace();
	}

	return bRes;
}

bool QWSDLParser::readTypes(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		logParser("processing: " + xmlReader.name().toString());

		if (xmlReader.name() == TAG_SCHEMA) {
			bRes = readSchema(xmlReader);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readMessage(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	m_pCurrentMessage = Message::create();
	if(xmlAttrs.hasAttribute(ATTR_NAME)) {
		m_pCurrentMessage->setLocalName(xmlAttrs.value(ATTR_NAME).toString());
		m_pCurrentMessage->setNamespace(m_szCurrentTargetNamespacePrefix);
		m_pCurrentMessage->setNamespaceUri(m_szCurrentTargetNamespaceUri);
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		logParser("processing: " + xmlReader.name().toString());

		if (xmlReader.name() == TAG_PART) {
			bRes = readPart(xmlReader);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// End element
	m_pListMessages->append(m_pCurrentMessage);
	m_pCurrentMessage.clear();

	return bRes;
}

bool QWSDLParser::readPortType(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		logParser("processing: " + xmlReader.name().toString());

		if (xmlReader.name() == TAG_OPERATION) {
			bRes = readOperation(xmlReader);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readPart(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	if(m_pCurrentMessage){
		if(xmlAttrs.hasAttribute(ATTR_NAME) && xmlAttrs.hasAttribute(ATTR_ELEMENT)) {
			if(xmlAttrs.value(ATTR_NAME).toString() == "parameters") {
				QString qualifiedName = xmlAttrs.value(ATTR_ELEMENT).toString();
				if(qualifiedName.contains(":")) {
					RequestResponseElementSharedPtr pElement;
					pElement = m_pListRequestResponseElements->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
					m_pCurrentMessage->setParameter(pElement);
				}
			}
		}
	}

	// Skip sub elements
	xmlReader.skipCurrentElement();

	return bRes;
}

bool QWSDLParser::readOperation(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QString qName = xmlReader.qualifiedName().toString();

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	if(qName.startsWith("wsdl:")){
		if(xmlAttrs.hasAttribute(ATTR_NAME)) {
			m_szCurrentOperationName = xmlAttrs.value(ATTR_NAME).toString();
			OperationSharedPtr pOperation = m_pListOperations->getByName(m_szCurrentOperationName);
			if(pOperation.isNull()) {
				m_pCurrentOperation = Operation::create();
				m_pCurrentOperation->setName(m_szCurrentOperationName);
			}
		}
	}else if(qName.startsWith("soap:")){
		OperationSharedPtr pOperation = m_pListOperations->getByName(m_szCurrentOperationName);
		if(pOperation) {
			m_pListOperations->removeAll(pOperation);
			pOperation->setSoapAction(xmlAttrs.value("soapAction").toString());
			m_pListOperations->append(pOperation);
			m_szCurrentOperationName = "";
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		logParser("processing: " + xmlReader.name().toString());

		if (xmlReader.name() == TAG_INPUT) {
			bRes = readInput(xmlReader);
		}else if (xmlReader.name() == TAG_OUTPUT) {
			bRes = readOutput(xmlReader);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// End element
	if(qName.startsWith("wsdl:")){
		if(m_pCurrentOperation) {
			m_pListOperations->append(m_pCurrentOperation);
			m_pService->addOperation(m_pCurrentOperation);
			m_pCurrentOperation.clear();
		}
	}

	return bRes;
}

bool QWSDLParser::readInput(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	if(m_pCurrentOperation){
		if(xmlAttrs.hasAttribute(ATTR_MESSAGE)) {
			QString qualifiedName = xmlAttrs.value(ATTR_MESSAGE).toString();
			if(qualifiedName.contains(":")) {
				MessageSharedPtr pMessage;
				pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
				m_pCurrentOperation->setInputMessage(pMessage);
			}
		}
	}

	// Skip sub elements
	xmlReader.skipCurrentElement();

	return bRes;
}

bool QWSDLParser::readOutput(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	if(m_pCurrentOperation){
		if(xmlAttrs.hasAttribute(ATTR_MESSAGE)) {
			QString qualifiedName = xmlAttrs.value(ATTR_MESSAGE).toString();
			if(qualifiedName.contains(":")) {
				MessageSharedPtr pMessage;
				pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
				m_pCurrentOperation->setOutputMessage(pMessage);
			}
		}
	}

	// Skip sub elements
	xmlReader.skipCurrentElement();

	return bRes;
}

bool QWSDLParser::readSchema(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QString szQualifiedName = xmlReader.qualifiedName().toString();
	m_szCurrentSchemaNamespacePrefix = szQualifiedName.split(":")[0];

	// Read XMLS namespaces
	bRes = readXMLNamespaces(xmlReader);

	// Parse attributes
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();
	QString szTargetNamespaceUri;
	if(xmlAttrs.hasAttribute(ATTR_TARGET_NAMESPACE)) {
		szTargetNamespaceUri = xmlAttrs.value(ATTR_TARGET_NAMESPACE).toString();

		// This is a special case used to find the SOAP standard fault type
		if(szTargetNamespaceUri == "http://www.w3.org/2003/05/soap-envelope"){
			m_bWaitForSoapEnvelopeFault = true;
		}

		m_pService->setTargetNamespace(szTargetNamespaceUri);
	}

	// Enter target namespace if any
	if(!szTargetNamespaceUri.isEmpty()){
		bRes = pushCurrentTargetNamespace(szTargetNamespaceUri);
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if(isComposition(szTagName)){
			bRes = readComposition(xmlReader, szTagName);
		}else if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, Section::Schema);
		}else if (xmlReader.name() == TAG_COMPLEX_TYPE) {
			bRes = readComplexType(xmlReader, Section::Schema);
		}else if (xmlReader.name() == TAG_ELEMENT) {
			bRes = readElement(xmlReader, Section::Schema);
		}else if (xmlReader.name() == TAG_ATTRIBUTE) {
			bRes = readAttribute(xmlReader, Section::Schema);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// Exit target namespace if any
	if(!szTargetNamespaceUri.isEmpty()){
		popCurrentTargetNamespace();
	}

	return bRes;
}

bool QWSDLParser::readComplexType(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	ComplexTypeSharedPtr pComplexType;

	if(iParentSection == Section::Element)
	{
		pComplexType = ComplexType::create();
		if(m_pCurrentElement){
			pComplexType->setLocalName(m_pCurrentElement->getName());
			pComplexType->setNamespace(m_szCurrentTargetNamespacePrefix);
			pComplexType->setNamespaceUri(m_szCurrentTargetNamespaceUri);
			m_pCurrentElement->setType(pComplexType);
		}
	}else{
		if(xmlAttrs.hasAttribute(ATTR_NAME))
		{
			TypeSharedPtr pFoundType;
			QString szName = xmlAttrs.value(ATTR_NAME).toString();

			if(szName == "NetworkCapabilities"){
				qDebug("break");
			}
			if(szName == "DeviceServiceCapabilities"){
				qDebug("break");
			}

			pFoundType = getTypeByName(szName, m_szCurrentTargetNamespacePrefix);
			if(!pFoundType.isNull()) {
				if(pFoundType->getClassType() == Type::TypeUnknown) {
					pComplexType = ComplexType::create();
					pComplexType->setLocalName(szName);
					pComplexType->setNamespace(m_szCurrentTargetNamespacePrefix);
					pComplexType->setNamespaceUri(m_szCurrentTargetNamespaceUri);
				}else{
					pComplexType = qSharedPointerCast<ComplexType>(pFoundType);;
				}
			}else{
				pComplexType = ComplexType::create();
				pComplexType->setLocalName(szName);
				pComplexType->setNamespace(m_szCurrentTargetNamespacePrefix);
				pComplexType->setNamespaceUri(m_szCurrentTargetNamespaceUri);
			}

			if((szName == "Fault") && m_bWaitForSoapEnvelopeFault){
				pComplexType->setIsSoapEnvelopeFault(true);
			}
		}
	}
	if(pComplexType){
		pushCurrentType(pComplexType);
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_RESTRICTION) {
			bRes = readRestriction(xmlReader, Section::ComplexType);
		}else if (xmlReader.name() == TAG_EXTENSION) {
			bRes = readExtension(xmlReader);
		}else if(isParticleAndAttrs(szTagName)){
			bRes = readParticleAndAttrs(xmlReader, szTagName, Section::ComplexType);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// End of element
	if(pComplexType){
		if((iParentSection == Section::Element) && m_pCurrentRequestResponseElement) {
			if(m_pCurrentElement && (m_pCurrentElement->getName() != m_pCurrentRequestResponseElement->getLocalName())){
				m_pListTypes->add(qSharedPointerCast<Type>(pComplexType));
			}

			m_pCurrentRequestResponseElement->setComplexType(pComplexType);

		}else{
			m_pListTypes->add(qSharedPointerCast<Type>(pComplexType));
		}

		if(m_pCurrentAttribute){
			m_pCurrentAttribute->setType(pComplexType);
		}

		popCurrentType();
	}

	return bRes;
}

bool QWSDLParser::readExtension(QXmlStreamReader& xmlReader)
{
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if(xmlAttrs.hasAttribute(ATTR_BASE)){
		QString szName = xmlAttrs.value(ATTR_BASE).toString();

		TypeSharedPtr pType = getTypeByName(szName.split(":")[1], szName.split(":")[0]);
		if(!pType.isNull()){
			qSharedPointerCast<ComplexType>(pCurrentType)->setExtensionType(pType);
		}else{
			if(szName.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
				SimpleTypeSharedPtr pType = SimpleType::create();
				pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szName);
				qSharedPointerCast<ComplexType>(pCurrentType)->setExtensionType(pType);
			}else{
				TypeSharedPtr pType = Type::create();
				pType->setNamespace(szName.split(":")[0]);
				pType->setLocalName(szName.split(":")[1]);
				m_pListTypes->append(pType);

				qSharedPointerCast<ComplexType>(pCurrentType)->setExtensionType(pType);
			}
		}
	}

	return true;
}

bool QWSDLParser::readElement(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	if(iParentSection == Section::ComplexType){
		TypeSharedPtr pCurrentType = getCurrentType();

		ElementSharedPtr pElement = Element::create();
		m_pCurrentElement = pElement;

		if(xmlAttrs.hasAttribute(ATTR_REF)){
			ElementSharedPtr pRefElement = getElementByRef(xmlAttrs.value(ATTR_REF).toString());
			if(pRefElement){
				pElement->setRef(pRefElement);
			}else{
				pElement->setRefValue(xmlAttrs.value(ATTR_REF).toString());
			}
		}else{
			if(xmlAttrs.hasAttribute(ATTR_NAME)) {
				pElement->setName(xmlAttrs.value(ATTR_NAME).toString());
			}

			if(xmlAttrs.hasAttribute(ATTR_TYPE)) {
				QString szValue = xmlAttrs.value(ATTR_TYPE).toString();
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];

				if(szLocalName == "NetworkCapabilities"){
					qDebug("break");
				}

				TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
				if(!pType.isNull()){
					pElement->setType(pType);
				}else{
					qWarning("[QWSDLParser] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
						pType->setName(pElement->getName());
						pElement->setType(pType);
					}else{
						TypeSharedPtr pType = Type::create();
						pType->setLocalName(szLocalName);
						pType->setNamespace(szNamespace);
						m_pListTypes->append(pType);
						pElement->setType(pType);
					}
				}

				if(pElement->getType()->getNameWithNamespace() == pCurrentType->getNameWithNamespace()){
					pElement->setIsNested(true);
				}
			}
		}
		//			else {
		//				if(attributes.index(ATTR_REF) != -1) {
		//					QString szValue = attributes.value(ATTR_REF);
		//					QString szNamespace = szValue.split(":")[0];
		//					QString szLocalName = szValue.split(":")[1];
		//
		//					TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
		//					if(!pType.isNull()){
		//						element->setType(pType);
		//					}else{
		//						qWarning("[QWSDLParser] Type %s is not found at this moment, we create it", qPrintable(szValue));
		//
		//						if(szValue.startsWith(m_szCurrentElementNamespacePrefix + ":")) {
		//							SimpleTypeSharedPtr pType = SimpleType::create();
		//							pType->setVariableTypeFromString(m_szCurrentElementNamespacePrefix, szValue);
		//							pType->setName(element->getName());
		//							element->setType(pType);
		//						}else{
		//							TypeSharedPtr pType = Type::create();
		//							pType->setLocalName(szLocalName);
		//							pType->setNamespace(szNamespace);
		//							m_pListTypes->append(pType);
		//							element->setType(pType);
		//						}
		//					}
		//				}
		//			}

		if(xmlAttrs.hasAttribute(ATTR_MIN_OCCURS)) {
			QString szValue = xmlAttrs.value(ATTR_MIN_OCCURS).toString();
			pElement->setMinOccurs(szValue.toUInt());
		}

		if(xmlAttrs.hasAttribute(ATTR_MAX_OCCURS)) {
			QString szValue = xmlAttrs.value(ATTR_MAX_OCCURS).toString();
			pElement->setMaxOccurs(szValue == "unbounded" ? -1 : szValue.toUInt());
		}

		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pCurrentType);
		pComplexType->addElement(pElement);
	}else{
		if(xmlAttrs.hasAttribute(ATTR_NAME) && (xmlAttrs.count() == 1)){
			//We are in Message Request/Response elements list section
			m_pCurrentRequestResponseElement = RequestResponseElement::create();
			m_pCurrentRequestResponseElement->setLocalName(xmlAttrs.value(ATTR_NAME).toString());
			m_pCurrentRequestResponseElement->setNamespace(m_szCurrentTargetNamespacePrefix);
			m_pCurrentRequestResponseElement->setNamespaceUri(m_szCurrentTargetNamespaceUri);
		}

		m_pCurrentElement = ElementSharedPtr::create();
		m_pCurrentElement->setNamespace(m_szCurrentTargetNamespacePrefix);
		if(xmlAttrs.hasAttribute(ATTR_NAME)){
			m_pCurrentElement->setName(xmlAttrs.value(ATTR_NAME).toString());
		}

		if(xmlAttrs.hasAttribute(ATTR_TYPE))
		{
			QString szValue = xmlAttrs.value(ATTR_TYPE).toString();
			QString szNamespace = szValue.split(":")[0];
			QString szLocalName = szValue.split(":")[1];
			TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
			if(!pType.isNull()){
				m_pCurrentElement->setType(pType);
			}else{
				qWarning("[QWSDLParser] Type %s is not found at this moment, we create it", qPrintable(szValue));

				if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
					pType->setName(m_pCurrentElement->getName());
					m_pCurrentElement->setType(pType);
				}else{
					TypeSharedPtr pType = Type::create();
					pType->setLocalName(szLocalName);
					pType->setNamespace(szNamespace);
					m_pListTypes->append(pType);
					m_pCurrentElement->setType(pType);
				}
			}
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, Section::Element);
		}else if (xmlReader.name() == TAG_COMPLEX_TYPE) {
			bRes = readComplexType(xmlReader, Section::Element);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	if(m_pCurrentElement){
		m_pListElements->append(m_pCurrentElement);
		m_pCurrentElement.clear();
	}

	if(m_pCurrentRequestResponseElement && !getCurrentType()){
		m_pListRequestResponseElements->append(m_pCurrentRequestResponseElement);
		m_pCurrentRequestResponseElement.clear();
	}

	return bRes;
}

bool QWSDLParser::isGroup(const QString& szTagName) const
{
	if(szTagName == TAG_GROUP){
		return true;
	}
	return false;
}

bool QWSDLParser::readGroup(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection)
{
	if(isMgs(szTagName)){
		return readMsg(xmlReader, szTagName, iParentSection);
	}
	return true;
}

bool QWSDLParser::readSequence(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_ELEMENT) {
			bRes = readElement(xmlReader, iParentSection);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readAny(QXmlStreamReader& xmlReader)
{
	// https://www.w3schools.com/xml/el_any.asp

	// Hack: we consider using simple type xs:string to allow developer to choose the proper value in use.
	// Only do it for empty type (never exists otherwise)
	//		if(inSection(Section::ComplexType) && currentType()){
	//			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(currentType());
	//			if(!pComplexType->getExtensionType() && pComplexType->getElementList()->isEmpty()){
	//				SimpleTypeSharedPtr pSimpleType = SimpleType::create();
	//				pSimpleType->setVariableType(SimpleType::AnyType);
	//				pComplexType->setExtensionType(pSimpleType);
	//			}
	//		}


	// Skip sub element
	xmlReader.skipCurrentElement();

	return true;
}

bool QWSDLParser::isAttribute(const QString& szTagName) const
{
	if(szTagName == TAG_ATTRIBUTE){
		return true;
	}
	return false;
}

bool QWSDLParser::readAttribute(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if((iParentSection == Section::ComplexType) && pCurrentType){
		QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

		AttributeSharedPtr attr = Attribute::create();

		if(xmlAttrs.hasAttribute(ATTR_REF)){
			AttributeSharedPtr pRefAttr = m_pListAttributes->getByRef(xmlAttrs.value(ATTR_REF).toString());
			if(pRefAttr){
				attr->setRef(pRefAttr);
			}
		}else{
			if(xmlAttrs.hasAttribute(ATTR_NAME)) {
				attr->setName(xmlAttrs.value(ATTR_NAME).toString());
			}

			if(xmlAttrs.hasAttribute(ATTR_TYPE)) {
				QString szValue = xmlAttrs.value(ATTR_TYPE).toString();
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];
				TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
				if(!pType.isNull()){
					attr->setType(pType);
				}else{
					qWarning("[QWSDLParser] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setName(attr->getName());
						pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
						attr->setType(pType);
					}else{
						TypeSharedPtr pType = Type::create();
						pType->setLocalName(szLocalName);
						pType->setNamespace(szNamespace);
						m_pListTypes->append(pType);
						attr->setType(pType);
					}
				}
			}else{
				m_pCurrentAttribute = attr;
				m_pCurrentAttribute->setNamespace(m_szCurrentTargetNamespacePrefix);
			}
		}

		if(xmlAttrs.hasAttribute(ATTR_USE)) {
			QString szValue = xmlAttrs.value(ATTR_USE).toString();
			if(szValue == "required") {
				attr->setRequired(true);
			}
		}

		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pCurrentType);
		pComplexType->addAttribute(attr);
	}else{
		m_pCurrentAttribute = AttributeSharedPtr::create();
		m_pCurrentAttribute->setNamespace(m_szCurrentTargetNamespacePrefix);
		if(xmlAttrs.hasAttribute(ATTR_NAME)){
			m_pCurrentAttribute->setName(xmlAttrs.value(ATTR_NAME).toString());
		}

		if(xmlAttrs.hasAttribute(ATTR_TYPE)) {
			QString szValue = xmlAttrs.value(ATTR_TYPE).toString();
			QString szNamespace = szValue.split(":")[0];
			QString szLocalName = szValue.split(":")[1];
			TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
			if(!pType.isNull()){
				m_pCurrentAttribute->setType(pType);
			}else{
				qWarning("[QWSDLParser] Type %s is not found at this moment, we create it", qPrintable(szValue));

				if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
					pType->setName(m_pCurrentAttribute->getName());
					m_pCurrentAttribute->setType(pType);
				}else{
					TypeSharedPtr pType = Type::create();
					pType->setLocalName(szLocalName);
					pType->setNamespace(szNamespace);
					m_pListTypes->append(pType);
					m_pCurrentAttribute->setType(pType);
				}
			}
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, Section::Attribute);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	if(m_pCurrentAttribute){
		m_pListAttributes->append(m_pCurrentAttribute);
		m_pCurrentAttribute.clear();
	}

	return bRes;
}

bool QWSDLParser::isAttributeGroup(const QString& szTagName) const
{
	if(szTagName == TAG_ATTRIBUTE_GROUP){
		return true;
	}
	return false;
}

bool QWSDLParser::readAttributeGroup(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_ATTRIBUTE) {
			bRes = readAttribute(xmlReader, iParentSection);
		}else if (xmlReader.name() == TAG_ATTRIBUTE_GROUP) {
			bRes = readAttributeGroup(xmlReader, iParentSection);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::isComposition(const QString& szTagName) const
{
	if(szTagName == TAG_INCLUDE){
		return true;
	}
	if(szTagName == TAG_IMPORT){
		return true;
	}
	if(szTagName == TAG_OVERRIDE){
		return true;
	}
	if(szTagName == TAG_REDEFINE){
		return true;
	}
	return false;
}

bool QWSDLParser::readComposition(QXmlStreamReader& xmlReader, const QString& szTagName)
{
	if(szTagName == TAG_INCLUDE){
		return readInclude(xmlReader);
	}else if(szTagName == TAG_IMPORT){
		return readImport(xmlReader);
	}else{
		xmlReader.skipCurrentElement();
	}
	return true;
}

bool QWSDLParser::readInclude(QXmlStreamReader& xmlReader)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	QString szLocation;
	QString szNamespaceUri;
	if(xmlAttrs.hasAttribute(ATTR_SCHEMA_LOCATION))
	{
		szLocation = xmlAttrs.value(ATTR_SCHEMA_LOCATION).toString();
	}
	if(xmlAttrs.hasAttribute(ATTR_NAMESPACE))
	{
		szNamespaceUri = xmlAttrs.value(ATTR_NAMESPACE).toString();
	}

	if(!szLocation.isEmpty()){
		logParser("starting import: " + szLocation);
		incrLogIndent();

		QString szRemoteLocation;
		if(szLocation.startsWith("http://") || szLocation.startsWith("https://")) {
			// Use URL
			szRemoteLocation = szLocation;
		}else if(!m_szCurrentTargetNamespaceUri.isEmpty()){
			// Build URL from current namespace URI
			if(m_szCurrentTargetNamespaceUri.startsWith("http://") || m_szCurrentTargetNamespaceUri.startsWith("https://"))
			{
				szRemoteLocation = m_szCurrentTargetNamespaceUri + (m_szCurrentTargetNamespaceUri.endsWith("/") ? szLocation : ("/" + szLocation));
			}
		}
		if(szNamespaceUri.isEmpty()){
			szNamespaceUri = m_szCurrentTargetNamespaceUri;
		}

		if(!szRemoteLocation.isEmpty()){
			bRes = loadFromHttp(szRemoteLocation, szNamespaceUri);
		}else{
			bRes = false;
		}
		if(!bRes){
			bRes = loadFromFile(szLocation, szNamespaceUri);
		}

		decrLogIndent();
		logParser("end of import");
	}

	// Skip sub elements
	xmlReader.skipCurrentElement();

	return bRes;
}

bool QWSDLParser::readImport(QXmlStreamReader& xmlReader)
{
	// Do the same as include
	return readInclude(xmlReader);
}


// Mgs
bool QWSDLParser::isMgs(const QString& szTagName) const
{
	if(szTagName == TAG_ALL){
		return true;
	}
	if(szTagName == TAG_CHOICE){
		return true;
	}
	if(szTagName == TAG_SEQUENCE){
		return true;
	}
	return false;
}

bool QWSDLParser::readMsg(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection)
{
	if(szTagName == TAG_SEQUENCE){
		return readSequence(xmlReader, iParentSection);
	}else{
		xmlReader.skipCurrentElement();
	}
	return true;
}

bool QWSDLParser::isAttrDecls(const QString& szTagName) const
{
	if(isAttribute(szTagName)){
		return true;
	}
	if(isAttributeGroup(szTagName)){
		return true;
	}
	return false;
}

bool QWSDLParser::readAttrDecls(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection)
{
	if(isAttribute(szTagName)){
		return readAttribute(xmlReader, iParentSection);
	}
	if(isAttributeGroup(szTagName)){
		return readAttributeGroup(xmlReader, iParentSection);
	}

	xmlReader.skipCurrentElement();

	return true;
}

bool QWSDLParser::isParticleAndAttrs(const QString& szTagName) const
{
	if(isMgs(szTagName)){
		return true;
	}
	if(isGroup(szTagName)){
		return true;
	}
	if(isAttrDecls(szTagName)){
		return true;
	}
	return false;
}

bool QWSDLParser::readParticleAndAttrs(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection)
{
	if(isMgs(szTagName)){
		return readMsg(xmlReader, szTagName, iParentSection);
	}
	if(isGroup(szTagName)){
		return readGroup(xmlReader, szTagName, iParentSection);
	}
	if(isAttrDecls(szTagName)){
		return readAttrDecls(xmlReader, szTagName, iParentSection);
	}
	xmlReader.skipCurrentElement();
	return true;
}

bool QWSDLParser::readSimpleType(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();
	QString szName;

	SimpleTypeSharedPtr pSimpleType;

	if((iParentSection == Section::Attribute) && m_pCurrentAttribute && (xmlAttrs.count() == 0)){
		TypeSharedPtr pCurrentType = SimpleType::create();
		m_pCurrentAttribute->setType(pCurrentType);
	}

	if(xmlAttrs.hasAttribute(ATTR_NAME)){
		szName = szName = xmlAttrs.value(ATTR_NAME).toString();
	}

	if(!szName.isEmpty()){
		TypeSharedPtr pFoundType = getTypeByName(szName, m_szCurrentTargetNamespacePrefix);
		if(!pFoundType.isNull()) {
			if(pFoundType->getClassType() == Type::TypeUnknown) {
				pSimpleType = SimpleType::create();
				pSimpleType->setLocalName(szName);
				pSimpleType->setNamespace(m_szCurrentTargetNamespacePrefix);
				pSimpleType->setNamespaceUri(m_szCurrentTargetNamespaceUri);
			}
		}else{
			pSimpleType = SimpleType::create();
			pSimpleType->setLocalName(szName);
			pSimpleType->setNamespace(m_szCurrentTargetNamespacePrefix);
		}
	}
	if(pSimpleType){
		pushCurrentType(pSimpleType);
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_RESTRICTION) {
			bRes = readRestriction(xmlReader, Section::SimpleType);
		}else if (xmlReader.name() == TAG_LIST) {
			bRes = readList(xmlReader, Section::SimpleType);
		}else if (xmlReader.name() == TAG_UNION) {
			bRes = readUnion(xmlReader, Section::SimpleType);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	// End of element
	if(pSimpleType && (pSimpleType->getClassType() == Type::TypeSimple))
	{
		m_pListTypes->add(qSharedPointerCast<Type>(pSimpleType));

		if(m_pCurrentAttribute){
			m_pCurrentAttribute->setType(pSimpleType);
		}

		popCurrentType();
	}

	return bRes;
}

bool QWSDLParser::readRestriction(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if((iParentSection == Section::SimpleType) &&
	(pCurrentType || (m_pCurrentAttribute &&
						  m_pCurrentAttribute->getType() &&
						  (m_pCurrentAttribute->getType()->getClassType() == Type::TypeSimple))))
	{
		SimpleTypeSharedPtr pSimpleType;
		if(pCurrentType){
			pSimpleType = qSharedPointerCast<SimpleType>(pCurrentType);
		}else{
			pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentAttribute->getType());
			if(pSimpleType){
				pSimpleType->setName(m_pCurrentAttribute->getName());
			}
		}

		if(pSimpleType){
			if(xmlAttrs.hasAttribute(ATTR_BASE)) {
				QString szValue = xmlAttrs.value(ATTR_BASE).toString();

				if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
					pSimpleType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
				}else{
					pSimpleType->setVariableType(SimpleType::Custom);
					pSimpleType->setCustomNamespace(szValue.split(":")[0].toUpper());
					pSimpleType->setCustomName(szValue.split(":")[1]);
				}
				pSimpleType->setRestricted(true);
			}
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, iParentSection);
		}else if(isFacet(szTagName)){
			bRes = readFacet(xmlReader, szTagName);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readList(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if((iParentSection == Section::Attribute) && m_pCurrentAttribute)
	{
		if(xmlAttrs.hasAttribute(ATTR_ITEM_TYPE)){
			QString szValue = xmlAttrs.value(ATTR_ITEM_TYPE).toString();
			if(iParentSection == Section::SimpleType){
				SimpleTypeSharedPtr pType = SimpleType::create();
				pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
				m_pCurrentAttribute->setType(pType);
				m_pCurrentAttribute->setIsList(true);
			}
		}
	}else if((iParentSection == Section::SimpleType) && pCurrentType)
	{
		if(xmlAttrs.hasAttribute(ATTR_ITEM_TYPE)){
			//				 We consider representing (for example):
			//
			//				<xs:simpleType name="IntAttrList">
			//					<xs:list itemType="xs:int"/>
			//				</xs:simpleType>
			//
			//				by:
			//				class IntAttrList : public QList<XS::Int> ...
			//
			//				So we admit it's a complex type with an extension. We remove the existing current type to build a new one.

			QString szValue = xmlAttrs.value(ATTR_ITEM_TYPE).toString();
			QString szLocalName = pCurrentType->getLocalName();
			QString szNamespace = pCurrentType->getNamespace();

			ComplexTypeSharedPtr pComplexType = ComplexType::create();
			pComplexType->setLocalName(szLocalName);
			pComplexType->setNamespace(szNamespace);

			TypeSharedPtr pType = getTypeByName(szValue.split(":")[1], szValue.split(":")[0]);
			if(!pType.isNull()){
				pComplexType->setExtensionType(pType, true);
			}else{
				if(szValue.startsWith(m_szCurrentSchemaNamespacePrefix + ":")) {
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szValue);
					pComplexType->setExtensionType(pType, true);
				}else{
					TypeSharedPtr pType = Type::create();
					pType->setNamespace(szValue.split(":")[0]);
					pType->setLocalName(szValue.split(":")[1]);
					m_pListTypes->append(pType);

					pComplexType->setExtensionType(pType, true);
				}
			}

			m_pListTypes->append(pComplexType);
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, iParentSection);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readUnion(QXmlStreamReader& xmlReader, Section::Name iParentSection)
{
	bool bRes = true;

	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	// For now, we consider using only the primitive simple type in the union.
	// Example: <xs:union memberTypes="tns:RelationshipType xs:anyURI"/> --> We consider only the xs:anyURI.
	if((iParentSection == Section::SimpleType) && pCurrentType){
		if(xmlAttrs.hasAttribute(ATTR_MEMBER_TYPES)){
			QString szMemberTypes = xmlAttrs.value(ATTR_MEMBER_TYPES).toString();
			QStringList szTypes = szMemberTypes.split(" ");
			for(int i = 0; i < szTypes.size(); ++i){
				if(szTypes[i].startsWith(m_szCurrentSchemaNamespacePrefix + ":")){
					SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pCurrentType);
					pSimpleType->setVariableTypeFromString(m_szCurrentSchemaNamespacePrefix, szTypes[i]);
				}
			}
		}
	}

	// Read sub elements
	incrLogIndent();
	while (bRes && xmlReader.readNextStartElement())
	{
		QString szTagName = xmlReader.name().toString();
		logParser("processing: " + szTagName);

		if (xmlReader.name() == TAG_SIMPLE_TYPE) {
			bRes = readSimpleType(xmlReader, iParentSection);
		}else{
			xmlReader.skipCurrentElement();
		}
	}
	decrLogIndent();

	return bRes;
}

bool QWSDLParser::readMaxLength(QXmlStreamReader& xmlReader)
{
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if(pCurrentType){
		if(xmlAttrs.hasAttribute(ATTR_VALUE)) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pCurrentType);
			QString szValue = xmlAttrs.value(ATTR_VALUE).toString();
			pSimpleType->setMaxLength(szValue.toUInt());
		}
	}

	// Skip sub element parsing
	xmlReader.skipCurrentElement();

	return true;
}

bool QWSDLParser::readMinLength(QXmlStreamReader& xmlReader)
{
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if(pCurrentType){
		if(xmlAttrs.hasAttribute(ATTR_VALUE)) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pCurrentType);
			QString szValue = xmlAttrs.value(ATTR_VALUE).toString();
			pSimpleType->setMinLength(szValue.toUInt());
		}
	}

	// Skip sub element parsing
	xmlReader.skipCurrentElement();

	return true;
}

bool QWSDLParser::readEnumeration(QXmlStreamReader& xmlReader)
{
	QXmlStreamAttributes xmlAttrs = xmlReader.attributes();

	TypeSharedPtr pCurrentType = getCurrentType();

	if(pCurrentType){
		if(xmlAttrs.hasAttribute(ATTR_VALUE)) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pCurrentType);
			pSimpleType->addEnumerationValue(xmlAttrs.value(ATTR_VALUE).toString());
		}
	}

	// Skip sub element parsing
	xmlReader.skipCurrentElement();

	return true;
}

bool QWSDLParser::isFacet(const QString& szTagName)
{
	if(szTagName == TAG_MAX_LENGTH){
		return true;
	}
	if(szTagName == TAG_MIN_LENGTH){
		return true;
	}
	if(szTagName == TAG_ENUMERATION){
		return true;
	}
	return false;
}

bool QWSDLParser::readFacet(QXmlStreamReader& xmlReader, const QString& szTagName)
{
	if(szTagName == TAG_MAX_LENGTH){
		return readMaxLength(xmlReader);
	}
	if(szTagName == TAG_MIN_LENGTH){
		return readMinLength(xmlReader);
	}
	if(szTagName == TAG_ENUMERATION){
		return readEnumeration(xmlReader);
	}

	// Skip sub element
	xmlReader.skipCurrentElement();

	return true;
}

/*
bool QWSDLParser::endElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName)
{
	switch(currentSection()){
	case Section::Definitions:
	{

	}
	break;
	case Section::Types:
	{

	}
	break;
	case Section::ComplexType:
	{
	}
	break;
	case Section::SimpleType:
	{
	}
	break;
	case Section::Attribute:
	{
		if(m_pCurrentAttribute){
			m_pListAttributes->append(m_pCurrentAttribute);
			m_pCurrentAttribute.clear();
		}
	}
	break;
	case Section::Element:
	{
		if(m_pCurrentElement){
			m_pListElements->append(m_pCurrentElement);
			m_pCurrentElement.clear();
		}

		if(m_pCurrentRequestResponseElement && !currentType()){
			m_pListRequestResponseElements->append(m_pCurrentRequestResponseElement);
			m_pCurrentRequestResponseElement.clear();
		}
	}
	break;
	case Section::Message:
	{
		m_pListMessages->append(m_pCurrentMessage);
		m_pCurrentMessage.clear();
	}
	break;
	case Section::Operation:
	{
		if(qName.startsWith("wsdl:")){
			if(m_pCurrentOperation) {
				m_pListOperations->append(m_pCurrentOperation);
				m_pService->addOperation(m_pCurrentOperation);
				m_pCurrentOperation.clear();
			}
		}
	}
	break;
	default:
		break;
	}

	leaveSection();

	return true;
}

bool
QWSDLParser::characters(const QString &str)
{
	m_szCurrentText = str.trimmed();
	return true;
}

bool
QWSDLParser::fatalError(const QXmlParseException &exception)
{
	qWarning("[QWSDLParser] Error while parsing XML content : (line %d) %s", exception.lineNumber(), qPrintable(exception.message()));
	return false;
}
*/
TypeListSharedPtr QWSDLParser::getTypeList() const
{
	return m_pListTypes;
}

RequestResponseElementListSharedPtr QWSDLParser::getRequestResponseElementList() const
{
	return m_pListRequestResponseElements;
}

ServiceSharedPtr QWSDLParser::getService() const
{
	return m_pService;
}

AttributeListSharedPtr QWSDLParser::getAttributeList() const
{
	return m_pListAttributes;
}

ElementListSharedPtr QWSDLParser::getElementList() const
{
	return m_pListElements;
}

ElementSharedPtr QWSDLParser::getElementByRef(const QString& szRef)
{
	ElementSharedPtr pElement = m_pListElements->getByRef(szRef);
	if(!pElement){
		QString szNamespace = szRef.split(":")[0];
		QString szName = szRef.split(":")[1];
		if(m_listNamespaceDeclarations.contains(szNamespace)){
			QString szNamespaceTmp = m_listNamespaceDeclarations.value(szNamespace);
			pElement = m_pListElements->getByRef(szNamespaceTmp + ":" + szName);
		}
	}
	return pElement;
}

TypeSharedPtr QWSDLParser::getTypeByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes)
{
	TypeSharedPtr pType = m_pListTypes->getByName(szLocalName, szNamespace, pListIgnoredTypes);
	if(!pType && m_listNamespaceDeclarations.contains(szNamespace)){
		QString szNamespaceTmp = m_listNamespaceDeclarations.value(szNamespace);
		pType = m_pListTypes->getByName(szLocalName, szNamespaceTmp, pListIgnoredTypes);
	}
	return pType;
}

bool QWSDLParser::loadFromHttp(const QString& szURL, const QString& szNamespaceUri)
{
	bool bRes = false;

	if(m_pDataLoaded->hasLoadedURI(szURL)){
		logParser("already loaded from http: " + szURL);
		return true;
	}

	logParser("loading from http: " + szURL);

	// Download the file
	QNetworkAccessManager manager;
	QNetworkReply* reply = manager.get(QNetworkRequest(QUrl::fromUserInput(szURL)));
	QEventLoop loop;
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	// Handle HTTP 301: redirected url
	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if(!possibleRedirectUrl.isNull()){
		QUrl redirectedUrl = possibleRedirectUrl.toUrl();
		if(!redirectedUrl.isEmpty()){
			reply = manager.get(QNetworkRequest(redirectedUrl));
			QEventLoop loop;
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
			loop.exec();
		}
	}

	// Load response
	QByteArray bytes(reply->readAll());

	// Parse WSDL
	QWSDLParser parser;
	parser.setInitialNamespaceDeclarationList(m_listNamespaceDeclarations);
	parser.setInitialNamespaceUri(szNamespaceUri);
	parser.setLogIndent(m_iLogIndent);
	parser.setWSDLData(m_pDataLoaded);
	QXmlStreamReader xmlReader;
	xmlReader.addData(bytes);
	bRes = parser.parse(xmlReader);
	if(bRes)
	{
		m_pDataLoaded->addLoadedURI(szURL);

		m_listNamespaceDeclarations = parser.getInitialNamespaceDeclarationList();

		TypeListSharedPtr pList = parser.getTypeList();
		TypeList::const_iterator type;
		for(type = pList->constBegin(); type != pList->constEnd(); ++type) {
			m_pListTypes->add(*type);
		}
		AttributeListSharedPtr pListAttributes = parser.getAttributeList();
		AttributeList::const_iterator attribute;
		for(attribute = pListAttributes->constBegin(); attribute != pListAttributes->constEnd(); ++attribute){
			m_pListAttributes->append(*attribute);
		}
		ElementListSharedPtr pListElements = parser.getElementList();
		ElementList::const_iterator element;
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element){
			m_pListElements->append(*element);
		}
	}else{
		qWarning("[QWSDLParser] Error to parse file %s (error: %s)",
				qPrintable(szURL),
				qPrintable(xmlReader.errorString()));
	}

	return bRes;
}

bool QWSDLParser::loadFromFile(const QString& szFileName, const QString& szNamespaceUri)
{
	bool bRes = true;

	if(m_pDataLoaded->hasLoadedURI(szFileName)){
		logParser("already loaded from file: " + szFileName);
		return true;
	}

	logParser("loading from file: " + szFileName);

	QFile file(szFileName);
	bRes = file.open(QFile::ReadOnly);
	if(bRes)
	{
		// Load response
		QByteArray bytes(file.readAll());

		// Parse WSDL
		QWSDLParser parser;
		parser.setInitialNamespaceDeclarationList(m_listNamespaceDeclarations);
		parser.setInitialNamespaceUri(szNamespaceUri);
		parser.setLogIndent(m_iLogIndent);
		parser.setWSDLData(m_pDataLoaded);
		QXmlStreamReader xmlReader;
		xmlReader.addData(bytes);
		bRes = parser.parse(xmlReader);
		if(bRes)
		{
			m_pDataLoaded->addLoadedURI(szFileName);

			m_listNamespaceDeclarations = parser.getInitialNamespaceDeclarationList();

			TypeListSharedPtr pList = parser.getTypeList();
			TypeList::const_iterator type;
			for(type = pList->constBegin(); type != pList->constEnd(); ++type) {
				m_pListTypes->add(*type);
			}
			AttributeListSharedPtr pListAttributes = parser.getAttributeList();
			AttributeList::const_iterator attribute;
			for(attribute = pListAttributes->constBegin(); attribute != pListAttributes->constEnd(); ++attribute){
				m_pListAttributes->append(*attribute);
			}
			ElementListSharedPtr pListElements = parser.getElementList();
			ElementList::const_iterator element;
			for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element){
				m_pListElements->append(*element);
			}
		}else{
			qWarning("[QWSDLParser] Error to parse file %s (error: %s)",
					qPrintable(szFileName),
					qPrintable(xmlReader.errorString()));
		}
	}else{
		qWarning("[QWSDLParser] Error for opening file %s (error: %s)",
				qPrintable(szFileName),
				qPrintable(file.errorString()));
	}

	return bRes;
}

void QWSDLParser::pushCurrentType(const TypeSharedPtr& pCurrentType)
{
	logParser(" add type: " + pCurrentType->getLocalName());
	m_stackCurrentTypes.push(pCurrentType);
}

void QWSDLParser::popCurrentType()
{
	logParser(" pop type:" + QString::number(m_stackCurrentTypes.count()));
	m_stackCurrentTypes.pop();
}

const TypeSharedPtr QWSDLParser::getCurrentType() const
{
	return !m_stackCurrentTypes.empty() ? m_stackCurrentTypes.top() : TypeSharedPtr(NULL);
}

bool QWSDLParser::pushCurrentTargetNamespace(const QString& szTargetNamespaceURI)
{
	bool bRes = true;

	m_stackTargetNamespace.push(szTargetNamespaceURI);

	updateTargetNamespacePrefix(szTargetNamespaceURI);

	return bRes;
}

void QWSDLParser::popCurrentTargetNamespace()
{
	QString szTargetNamespace;

	m_stackTargetNamespace.pop();

	if(!m_stackTargetNamespace.isEmpty()){
		szTargetNamespace = m_stackTargetNamespace.top();
	}

	updateTargetNamespacePrefix(szTargetNamespace);
}

void QWSDLParser::updateTargetNamespacePrefix(const QString& szTargetNamespaceURI)
{
	QMap<QString, QString>::const_iterator iter;
	iter = m_listNamespaceDeclarations.constBegin();
	while(iter != m_listNamespaceDeclarations.constEnd())
	{
		const QString& szPrefix = iter.key();
		const QString& szNamespaceURI = iter.value();
		if(szNamespaceURI == szTargetNamespaceURI)
		{
			m_szCurrentTargetNamespacePrefix = szPrefix;
			m_szCurrentTargetNamespaceUri = szNamespaceURI;
			qDebug("[QWSDLParser] Target namespace prefix found in definitions: %s", qPrintable(m_szCurrentTargetNamespacePrefix));
			break;
		}
		iter++;
	}
}

void QWSDLParser::logParser(const QString& szMsg)
{
	QString szIdent;
	for(int i=0; i<m_iLogIndent; i++)
	{
		szIdent += " ";
	}
	qDebug("[QWSDLParser] %s%s", qPrintable(szIdent), qPrintable(szMsg));
}

void QWSDLParser::incrLogIndent()
{
	m_iLogIndent++;
}

void QWSDLParser::decrLogIndent()
{
	m_iLogIndent--;
}