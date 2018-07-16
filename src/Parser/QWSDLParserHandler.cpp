/*
 * QWSDLParserHandler.cpp
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include "Model/ComplexType.h"
#include "Model/SimpleType.h"
#include "Model/Type.h"
#include "QWSDLParserHandler.h"
#include "WSDLAttributes.h"

QWSDLParserHandler::QWSDLParserHandler(QMap<QString, QStringList>* pNamespaceRoutingMap, const QString& szOutNamespace)
	: m_pNamespaceRoutingMap(pNamespaceRoutingMap), m_szOutNamespace(szOutNamespace)
{
	if(!m_pNamespaceRoutingMap){
		m_pNamespaceRoutingMap = new QMap<QString, QStringList>();
		m_bOwnNamespaceRoutingMap = true;
	}else{
		m_bOwnNamespaceRoutingMap = false;
	}

	m_pService = Service::create();
	m_pListTypes = TypeList::create();
	m_pListRequestResponseElements = RequestResponseElementList::create();
	m_pListMessages = MessageList::create();
	m_pListOperations = OperationList::create();
	m_pListAttributes = AttributeList::create();
	m_pListElements = ElementList::create();

	initXMLAttributes();
}

QWSDLParserHandler::~QWSDLParserHandler()
{
	if(m_bOwnNamespaceRoutingMap && m_pNamespaceRoutingMap){
		delete m_pNamespaceRoutingMap;
		m_pNamespaceRoutingMap = NULL;
	}
}

void QWSDLParserHandler::initXMLAttributes()		// https://www.w3.org/2001/xml.xsd
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

bool QWSDLParserHandler::endDocument()
{
	TypeSharedPtr pType;
	TypeList::const_iterator type;
	TypeListSharedPtr pListTypesToRemove = TypeList::create();;

	RequestResponseElementList::iterator requestResponseElement;

	ElementList::iterator element;
	ElementSharedPtr pElement;

	AttributeList::iterator attr;
	AttributeSharedPtr pAttribute;

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
					if(pElement->getType()->getClassType() == Type::TypeUnknown) {
						pType = getTypeByName(pElement->getType()->getLocalName(), pElement->getType()->getNamespace(), pListTypesToRemove);
						while(pType->getClassType() == Type::TypeUnknown) {
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

					if(pAttribute->getType()->getClassType() == Type::TypeUnknown){
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
					element != pComplexType->getElementList()->end(); ++element) {

				if((*element)->hasRef()){
					pElement = (*element)->getRef();
				}else{
					pElement = *element;
				}

				if(pElement->getType()->getClassType() == Type::TypeUnknown) {
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

bool QWSDLParserHandler::startElement(const QString &namespaceURI,
		const QString &localName,
		const QString &qName,
		const QXmlAttributes &attributes)
{
	enterSection(localName);

	switch(currentSection()){
	/*
	 * definitions
	 */
	case Section::Definitions:
	{
		QString szTargetNamespace;

		if(attributes.index(ATTR_NAME) != -1) {
			m_pService->setName(attributes.value(ATTR_NAME));
		}

		if(attributes.index(ATTR_TARGET_NAMESPACE) != -1){
			szTargetNamespace = attributes.value(ATTR_TARGET_NAMESPACE);
			m_pService->setTargetNamespace(szTargetNamespace);
		}

		for(int i=0; i < attributes.length(); ++i) {
			if(attributes.value(i) == szTargetNamespace && attributes.qName(i) != ATTR_TARGET_NAMESPACE) {
				m_szTargetNamespacePrefix = attributes.localName(i);
				qDebug("[QWSDLParserHandler::startElement] Target namespace prefix found in definitions: %s", qPrintable(m_szTargetNamespacePrefix));
			}
		}
	}
	break;
	/*
	 * schema
	 */
	case Section::Schema:
	{
		m_szCurrentNamespacePrefix = qName.split(":")[0];

		QString szTargetNamespace;

		if(attributes.index(ATTR_TARGET_NAMESPACE) != -1) {
			szTargetNamespace = attributes.value(ATTR_TARGET_NAMESPACE);
			m_pService->setTargetNamespace(szTargetNamespace);
		}

		for(int i=0; i < attributes.length(); ++i) {
			if(attributes.qName(i).startsWith("xmlns:")){
				m_pNamespaceDeclarationMap.insert(attributes.localName(i), attributes.value(i));
			}

			if(attributes.value(i) == szTargetNamespace && attributes.qName(i) != ATTR_TARGET_NAMESPACE){
				m_szTargetNamespacePrefix = attributes.localName(i);

				if(!m_szOutNamespace.isEmpty()){
					QStringList namespaceList = m_pNamespaceRoutingMap->value(m_szOutNamespace);
					if(!namespaceList.contains(m_szTargetNamespacePrefix)){
						namespaceList.append(m_szTargetNamespacePrefix);
						m_pNamespaceRoutingMap->insert(m_szOutNamespace, namespaceList);
					}
				}

				qDebug("[QWSDLParserHandler::startElement] Target namespace prefix found in schema: %s", qPrintable(m_szTargetNamespacePrefix));
			}
		}
	}
	break;
	/*
	 * import | include
	 */
	case Section::Import:
	case Section::Include:
	{
		if(attributes.index(ATTR_SCHEMA_LOCATION) != -1) {
			QString szLocation(attributes.value(ATTR_SCHEMA_LOCATION));
			QString szNamespace(attributes.value(ATTR_NAMESPACE));

			QString szRoutedNamespace;
			QMapIterator<QString, QString> iter(m_pNamespaceDeclarationMap);
			while(iter.hasNext()){
				iter.next();
				if(iter.value() == szNamespace){
					szRoutedNamespace = iter.key();
					break;
				}
			}

			if(!szRoutedNamespace.isEmpty() && !m_pNamespaceRoutingMap->contains(szRoutedNamespace)){
				m_pNamespaceRoutingMap->insert(szRoutedNamespace, QStringList());
			}

			if(szLocation.startsWith("http://") || szLocation.startsWith("https://")) {
				loadFromHttp(szLocation, szRoutedNamespace);
			}else if(szNamespace.startsWith("http://") || szNamespace.startsWith("https://")){
				loadFromHttp(szNamespace + (szNamespace.endsWith("/") ? szLocation : ("/" + szLocation)), szRoutedNamespace);
			}else{
				loadFromFile(szLocation, szRoutedNamespace);
			}
		}
	}
	break;
	/*
	 * complexType
	 */
	case Section::ComplexType:
	{
		if(inSection(Section::Element)){
			ComplexTypeSharedPtr pComplexType = ComplexType::create();
			if(m_pCurrentElement){
				pComplexType->setLocalName(m_pCurrentElement->getName());
				pComplexType->setNamespace(m_szTargetNamespacePrefix);
				m_pCurrentElement->setType(pComplexType);
			}
			addCurrentType(pComplexType);
		}else{
			if(attributes.index(ATTR_NAME) != -1) {
				TypeSharedPtr pCurrentType;
				QString szName = attributes.value(ATTR_NAME);
				if(!getTypeByName(szName, m_szTargetNamespacePrefix).isNull()) {
					pCurrentType = getTypeByName(szName, m_szTargetNamespacePrefix);
					if(pCurrentType->getClassType() == Type::TypeUnknown) {
						pCurrentType = ComplexType::create();
						pCurrentType->setLocalName(szName);
						pCurrentType->setNamespace(m_szTargetNamespacePrefix);
					}
				}else{
					pCurrentType = ComplexType::create();
					pCurrentType->setLocalName(szName);
					pCurrentType->setNamespace(m_szTargetNamespacePrefix);
				}
				addCurrentType(pCurrentType);
			}
		}
	}
	break;
	/*
	 * simpleType
	 */
	case Section::SimpleType:
	{
		QString szName;

		if(inSection(Section::Attribute) && m_pCurrentAttribute && (attributes.count() == 0)){
			TypeSharedPtr pCurrentType = SimpleType::create();
			m_pCurrentAttribute->setType(pCurrentType);
		}

		if(attributes.index(ATTR_NAME) != -1){
			szName = szName = attributes.value(ATTR_NAME);
		}

		if(!szName.isEmpty()){
			TypeSharedPtr pCurrentType;
			if(!getTypeByName(szName, m_szTargetNamespacePrefix).isNull()) {
				pCurrentType = getTypeByName(szName, m_szTargetNamespacePrefix);
				if(pCurrentType->getClassType() == Type::TypeUnknown) {
					pCurrentType = SimpleType::create();
					pCurrentType->setLocalName(szName);
					pCurrentType->setNamespace(m_szTargetNamespacePrefix);
				}
			}else{
				pCurrentType = SimpleType::create();
				pCurrentType->setLocalName(szName);
				pCurrentType->setNamespace(m_szTargetNamespacePrefix);
			}
			addCurrentType(pCurrentType);
		}
	}
	break;
	/*
	 * attribute
	 */
	case Section::Attribute:
	{
		if(inSection(Section::ComplexType) && currentType()){
			AttributeSharedPtr attr = Attribute::create();

			if(attributes.index(ATTR_REF) != -1){
				AttributeSharedPtr pRefAttr = m_pListAttributes->getByRef(attributes.value(ATTR_REF));
				if(pRefAttr){
					attr->setRef(pRefAttr);
				}
			}else{
				if(attributes.index(ATTR_NAME) != -1) {
					attr->setName(attributes.value(ATTR_NAME));
				}

				if(attributes.index(ATTR_TYPE) != -1) {
					QString szValue = attributes.value(ATTR_TYPE);
					QString szNamespace = szValue.split(":")[0];
					QString szLocalName = szValue.split(":")[1];
					TypeSharedPtr pType = getTypeByName(szNamespace, szLocalName);
					if(!pType.isNull()){
						attr->setType(pType);
					}else{
						qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

						if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
							SimpleTypeSharedPtr pType = SimpleType::create();
							pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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
				}else{
					m_pCurrentAttribute = attr;
					m_pCurrentAttribute->setNamespace(m_szTargetNamespacePrefix);
				}
			}

			if(attributes.index(ATTR_USE) != -1) {
				QString szValue = attributes.value(ATTR_USE);
				if(szValue == "required") {
					attr->setRequired(true);
				}
			}

			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(currentType());
			pComplexType->addAttribute(attr);
		}else{
			m_pCurrentAttribute = AttributeSharedPtr::create();
			m_pCurrentAttribute->setNamespace(m_szTargetNamespacePrefix);
			if(attributes.index(ATTR_NAME) != -1){
				m_pCurrentAttribute->setName(attributes.value(ATTR_NAME));
			}

			if(attributes.index(ATTR_TYPE) != -1) {
				QString szValue = attributes.value(ATTR_TYPE);
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];
				TypeSharedPtr pType = getTypeByName(szNamespace, szLocalName);
				if(!pType.isNull()){
					m_pCurrentAttribute->setType(pType);
				}else{
					qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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
	}
	break;
	/*
	 * extension
	 */
	case Section::Extension:
	{
		if(inSection(Section::ComplexType) && currentType()){
			if(attributes.index(ATTR_BASE) != -1){
				QString szName = attributes.value(ATTR_BASE);

				TypeSharedPtr pType = getTypeByName(szName.split(":")[1], szName.split(":")[0]);
				if(!pType.isNull()){
					qSharedPointerCast<ComplexType>(currentType())->setExtensionType(pType);
				}else{
					if(szName.startsWith(m_szCurrentNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szName);
						qSharedPointerCast<ComplexType>(currentType())->setExtensionType(pType);
					}else{
						TypeSharedPtr pType = Type::create();
						pType->setNamespace(szName.split(":")[0]);
						pType->setLocalName(szName.split(":")[1]);
						m_pListTypes->append(pType);

						qSharedPointerCast<ComplexType>(currentType())->setExtensionType(pType);
					}
				}
			}
		}
	}
	break;
	/*
	 * element
	 */
	case Section::Element:
	{
		if(inSection(Section::ComplexType) && currentType()){
			ElementSharedPtr element = Element::create();
			m_pCurrentElement = element;

			if(attributes.index(ATTR_REF) != -1){
				ElementSharedPtr pRefElement = getElementByRef(attributes.value(ATTR_REF));
				if(pRefElement){
					element->setRef(pRefElement);
				}else{
					element->setRefValue(attributes.value(ATTR_REF));
				}
			}else{
				if(attributes.index(ATTR_NAME) != -1) {
					element->setName(attributes.value(ATTR_NAME));
				}

				if(attributes.index(ATTR_TYPE) != -1) {
					QString szValue = attributes.value(ATTR_TYPE);
					QString szNamespace = szValue.split(":")[0];
					QString szLocalName = szValue.split(":")[1];

					TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
					if(!pType.isNull()){
						element->setType(pType);
					}else{
						qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

						if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
							SimpleTypeSharedPtr pType = SimpleType::create();
							pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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

					if(element->getType()->getNameWithNamespace() == currentType()->getNameWithNamespace()){
						element->setIsNested(true);
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
//						qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));
//
//						if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
//							SimpleTypeSharedPtr pType = SimpleType::create();
//							pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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

			if(attributes.index(ATTR_MIN_OCCURS) != -1) {
				element->setMinOccurs(attributes.value(ATTR_MIN_OCCURS).toUInt());
			}

			if(attributes.index(ATTR_MAX_OCCURS) != -1) {
				QString szValue = attributes.value(ATTR_MAX_OCCURS);
				element->setMaxOccurs(szValue == "unbounded" ? -1 : szValue.toUInt());
			}

			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(currentType());
			pComplexType->addElement(element);
		}else{
			if((attributes.index(ATTR_NAME) != -1) && (attributes.count() == 1)){
				//We are in Message Request/Response elements list section
				m_pCurrentRequestResponseElement = RequestResponseElement::create();
				m_pCurrentRequestResponseElement->setLocalName(attributes.value(ATTR_NAME));
				m_pCurrentRequestResponseElement->setNamespace(m_szTargetNamespacePrefix);
			}

			m_pCurrentElement = ElementSharedPtr::create();
			m_pCurrentElement->setNamespace(m_szTargetNamespacePrefix);
			if(attributes.index(ATTR_NAME) != -1){
				m_pCurrentElement->setName(attributes.value(ATTR_NAME));
			}

			if(attributes.index(ATTR_TYPE) != -1) {
				QString szValue = attributes.value(ATTR_TYPE);
				QString szNamespace = szValue.split(":")[0];
				QString szLocalName = szValue.split(":")[1];
				TypeSharedPtr pType = getTypeByName(szLocalName, szNamespace);
				if(!pType.isNull()){
					m_pCurrentElement->setType(pType);
				}else{
					qWarning("[QWSDLParserHandler::startElement] Type %s is not found at this moment, we create it", qPrintable(szValue));

					if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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
	}
	break;
	/*
	 * part
	 */
	case Section::Part:
	{
		if(inSection(Section::Message) && m_pCurrentMessage){
			if(attributes.index(ATTR_NAME) != -1 && attributes.index(ATTR_ELEMENT) != -1) {
				if(attributes.value(ATTR_NAME) == "parameters") {
					QString qualifiedName = attributes.value(ATTR_ELEMENT);
					if(qualifiedName.contains(":")) {
						RequestResponseElementSharedPtr pElement;
						pElement = m_pListRequestResponseElements->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
						m_pCurrentMessage->setParameter(pElement);
					}
				}
			}
		}
	}
	break;
	/*
	 * input
	 */
	case Section::Input:
	{
		if(inSection(Section::Operation) && m_pCurrentOperation){
			if(attributes.index(ATTR_MESSAGE) != -1) {
				QString qualifiedName = attributes.value(ATTR_MESSAGE);
				if(qualifiedName.contains(":")) {
					MessageSharedPtr pMessage;
					pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
					m_pCurrentOperation->setInputMessage(pMessage);
				}
			}
		}
	}
	break;
	/*
	 * output
	 */
	case Section::Output:
	{
		if(inSection(Section::Operation) && m_pCurrentOperation){
			if(attributes.index(ATTR_MESSAGE) != -1) {
				QString qualifiedName = attributes.value(ATTR_MESSAGE);
				if(qualifiedName.contains(":")) {
					MessageSharedPtr pMessage;
					pMessage = m_pListMessages->getByName(qualifiedName.split(":")[1], qualifiedName.split(":")[0]);
					m_pCurrentOperation->setOutputMessage(pMessage);
				}
			}
		}
	}
	break;
	/*
	 * message
	 */
	case Section::Message:
	{
		m_pCurrentMessage = Message::create();
		if(attributes.index(ATTR_NAME) != -1) {
			m_pCurrentMessage->setLocalName(attributes.value(ATTR_NAME));
			m_pCurrentMessage->setNamespace(m_szTargetNamespacePrefix);
		}
	}
	break;
	/*
	 * operation
	 */
	case Section::Operation:
	{
		if(qName.startsWith("wsdl:")){
			if(attributes.index(ATTR_NAME) != -1) {
				m_szCurrentOperationName = attributes.value(ATTR_NAME);
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
				pOperation->setSoapAction(attributes.value("soapAction"));
				m_pListOperations->append(pOperation);
				m_szCurrentOperationName = "";
			}
		}
	}
	break;
	/*
	 * restriction
	 */
	case Section::Restriction:
	{
		if(inSection(Section::SimpleType) &&
				(currentType() || (m_pCurrentAttribute &&
						m_pCurrentAttribute->getType() &&
						(m_pCurrentAttribute->getType()->getClassType() == Type::TypeSimple)))){
			SimpleTypeSharedPtr pSimpleType;
			if(currentType()){
				pSimpleType = qSharedPointerCast<SimpleType>(currentType());
			}else{
				pSimpleType = qSharedPointerCast<SimpleType>(m_pCurrentAttribute->getType());
				if(pSimpleType){
					pSimpleType->setName(m_pCurrentAttribute->getName());
				}
			}

			if(pSimpleType){
				if(attributes.index(ATTR_BASE) != -1) {
					QString szValue = attributes.value(ATTR_BASE);

					if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
						pSimpleType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
					}else{
						pSimpleType->setVariableType(SimpleType::Custom);
						pSimpleType->setCustomNamespace(szValue.split(":")[0].toUpper());
						pSimpleType->setCustomName(szValue.split(":")[1]);
					}
					pSimpleType->setRestricted(true);
				}
			}
		}
	}
	break;
	/*
	 * maxLength
	 */
	case Section::MaxLength:
	{
		if(inSection(Section::SimpleType) && currentType()){
			if(attributes.index(ATTR_VALUE) != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(currentType());
				pSimpleType->setMaxLength(attributes.value(ATTR_VALUE).toUInt());
			}
		}
	}
	break;
	/*
	 * minLength
	 */
	case Section::MinLength:
	{
		if(inSection(Section::SimpleType) && currentType()){
			if(attributes.index(ATTR_VALUE) != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(currentType());
				pSimpleType->setMaxLength(attributes.value(ATTR_VALUE).toUInt());
			}
		}
	}
	break;
	/*
	 * enumeration
	 */
	case Section::Enumeration:
	{
		if(inSection(Section::SimpleType) && currentType()){
			if(attributes.index(ATTR_VALUE) != -1) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(currentType());
				pSimpleType->addEnumerationValue(attributes.value(ATTR_VALUE));
			}
		}
	}
	break;
	/*
	 * list
	 */
	case Section::List:
	{
		if(inSection(Section::Attribute) && m_pCurrentAttribute){
			if(attributes.index(ATTR_ITEM_TYPE) != -1){
				QString szValue = attributes.value(ATTR_ITEM_TYPE);
				if(parentSection() == Section::SimpleType){
					SimpleTypeSharedPtr pType = SimpleType::create();
					pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
					m_pCurrentAttribute->setType(pType);
					m_pCurrentAttribute->setIsList(true);
				}
			}
		}else if((parentSection() == Section::SimpleType) && currentType()){
			if(attributes.index(ATTR_ITEM_TYPE) != -1){
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

				QString szValue = attributes.value(ATTR_ITEM_TYPE);
				QString szLocalName = currentType()->getLocalName();
				QString szNamespace = currentType()->getNamespace();
				leaveCurrentType();

				ComplexTypeSharedPtr pComplexType = ComplexType::create();
				pComplexType->setLocalName(szLocalName);
				pComplexType->setNamespace(szNamespace);

				TypeSharedPtr pType = getTypeByName(szValue.split(":")[1], szValue.split(":")[0]);
				if(!pType.isNull()){
					pComplexType->setExtensionType(pType, true);
				}else{
					if(szValue.startsWith(m_szCurrentNamespacePrefix + ":")) {
						SimpleTypeSharedPtr pType = SimpleType::create();
						pType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szValue);
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
	}
	break;
	/*
	 * union
	 */
	case Section::Union:
	{
		// For now, we consider using only the primitive simple type in the union.
		// Example: <xs:union memberTypes="tns:RelationshipType xs:anyURI"/> --> We consider only the xs:anyURI.
		if((parentSection() == Section::SimpleType) && currentType()){
			if(attributes.index(ATTR_MEMBER_TYPES) != -1){
				QString szMemberTypes = attributes.value(ATTR_MEMBER_TYPES);
				QStringList szTypes = szMemberTypes.split(" ");
				for(int i = 0; i < szTypes.size(); ++i){
					if(szTypes[i].startsWith(m_szCurrentNamespacePrefix + ":")){
						SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(currentType());
						pSimpleType->setVariableTypeFromString(m_szCurrentNamespacePrefix, szTypes[i]);
					}
				}
			}
		}
	}
	break;
	case Section::Any:		// https://www.w3schools.com/xml/el_any.asp
	{
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
	}
	break;
	default:
		break;
	}

	return true;
}

bool QWSDLParserHandler::endElement(const QString &namespaceURI,
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
		if(currentType()){
			if(inSection(Section::Element) && m_pCurrentRequestResponseElement) {
				if(m_pCurrentElement && (m_pCurrentElement->getName() != m_pCurrentRequestResponseElement->getLocalName())){
					m_pListTypes->add(currentType());
				}

				m_pCurrentRequestResponseElement->setComplexType(qSharedPointerCast<ComplexType>(currentType()));

				if(m_pCurrentAttribute){
					m_pCurrentAttribute->setType(currentType());
				}

				leaveCurrentType();
			}else{
				m_pListTypes->add(currentType());

				if(!m_pCurrentAttribute.isNull()){
					m_pCurrentAttribute->setType(currentType());
				}

				leaveCurrentType();
			}
		}
	}
	break;
	case Section::SimpleType:
	{
		if(currentType() && (currentType()->getClassType() == Type::TypeSimple)){
			m_pListTypes->add(currentType());

			if(m_pCurrentAttribute){
				m_pCurrentAttribute->setType(currentType());
			}

			leaveCurrentType();
		}
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

RequestResponseElementListSharedPtr QWSDLParserHandler::getRequestResponseElementList() const
{
	return m_pListRequestResponseElements;
}

ServiceSharedPtr QWSDLParserHandler::getService() const
{
	return m_pService;
}

AttributeListSharedPtr QWSDLParserHandler::getAttributeList() const
{
	return m_pListAttributes;
}

ElementListSharedPtr QWSDLParserHandler::getElementList() const
{
	return m_pListElements;
}

ElementSharedPtr QWSDLParserHandler::getElementByRef(const QString& szRef)
{
	ElementSharedPtr pElement = m_pListElements->getByRef(szRef);
	if(!pElement){
		QString szNamespace = szRef.split(":")[0];
		QString szName = szRef.split(":")[1];
		if(m_pNamespaceRoutingMap->contains(szNamespace)){
			QStringList namespaceList = m_pNamespaceRoutingMap->value(szNamespace);
			for(int i = 0; i < namespaceList.size(); ++i){
				QString szNamespaceTmp = namespaceList[i];
				pElement = m_pListElements->getByRef(szNamespaceTmp + ":" + szName);
				if(pElement){
					break;
				}
			}
		}
	}
	return pElement;
}

TypeSharedPtr QWSDLParserHandler::getTypeByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes)
{
	TypeSharedPtr pType = m_pListTypes->getByName(szLocalName, szNamespace, pListIgnoredTypes);
	if(!pType && m_pNamespaceRoutingMap->contains(szNamespace)){
		QStringList namespaceList = m_pNamespaceRoutingMap->value(szNamespace);
		for(int i = 0; i < namespaceList.size(); ++i){
			QString szNamespaceTmp = namespaceList[i];
			pType = m_pListTypes->getByName(szLocalName, szNamespaceTmp, pListIgnoredTypes);
			if(pType){
				break;
			}
		}
	}
	return pType;
}

void QWSDLParserHandler::loadFromHttp(const QString& szURL, const QString& szNamespace)
{
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

	QByteArray bytes(reply->readAll());
	QBuffer buffer;
	buffer.setData(bytes);

	QXmlInputSource source(&buffer);
	QXmlSimpleReader reader;
	QWSDLParserHandler handler(m_pNamespaceRoutingMap, szNamespace);
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
		AttributeListSharedPtr pListAttributes = handler.getAttributeList();
		AttributeList::const_iterator attribute;
		for(attribute = pListAttributes->constBegin(); attribute != pListAttributes->constEnd(); ++attribute){
			m_pListAttributes->append(*attribute);
		}
		ElementListSharedPtr pListElements = handler.getElementList();
		ElementList::const_iterator element;
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element){
			m_pListElements->append(*element);
		}
	}else{
		qWarning("[QWSDLParserHandler::startElement] Error to parse file %s (error: %s)",
				qPrintable(szURL),
				qPrintable(reader.errorHandler()->errorString()));
	}
}

void QWSDLParserHandler::loadFromFile(const QString& szFileName, const QString& szNamespace)
{
	QFile file(szFileName);
	if(file.open(QFile::ReadOnly)) {

		QByteArray bytes = file.readAll();
		QBuffer buffer;
		buffer.setData(bytes);

		QXmlInputSource source(&buffer);
		QXmlSimpleReader reader;
		QWSDLParserHandler handler(m_pNamespaceRoutingMap, szNamespace);
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
			AttributeListSharedPtr pListAttributes = handler.getAttributeList();
			AttributeList::const_iterator attribute;
			for(attribute = pListAttributes->constBegin(); attribute != pListAttributes->constEnd(); ++attribute){
				m_pListAttributes->append(*attribute);
			}
			ElementListSharedPtr pListElements = handler.getElementList();
			ElementList::const_iterator element;
			for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element){
				m_pListElements->append(*element);
			}
		}else{
			qWarning("[QWSDLParserHandler::startElement] Error to parse file %s (error: %s)",
					qPrintable(szFileName),
					qPrintable(reader.errorHandler()->errorString()));
		}
	}else{
		qWarning("[QWSDLParserHandler::startElement] Error for opening file %s (error: %s)",
				qPrintable(szFileName),
				qPrintable(file.errorString()));
	}
}

void QWSDLParserHandler::addCurrentType(const TypeSharedPtr& pCurrentType)
{
	m_currentTypeStack.push(pCurrentType);
}

void QWSDLParserHandler::leaveCurrentType()
{
	m_currentTypeStack.pop();
}

const TypeSharedPtr QWSDLParserHandler::currentType() const
{
	return !m_currentTypeStack.empty() ? m_currentTypeStack.top() : TypeSharedPtr(NULL);
}

void QWSDLParserHandler::enterSection(QString szSection)
{
	if(szSection == TAG_DEFINITIONS){
		enterSection(Section::Definitions);
	}else if(szSection == TAG_TYPES){
		enterSection(Section::Types);
	}else if(szSection == TAG_SCHEMA){
		enterSection(Section::Schema);
	}else if(szSection == TAG_IMPORT){
		enterSection(Section::Import);
	}else if(szSection == TAG_INCLUDE){
		enterSection(Section::Include);
	}else if(szSection == TAG_COMPLEX_TYPE){
		enterSection(Section::ComplexType);
	}else if(szSection == TAG_SIMPLE_TYPE){
		enterSection(Section::SimpleType);
	}else if(szSection == TAG_ATTRIBUTE){
		enterSection(Section::Attribute);
	}else if(szSection == TAG_EXTENSION){
		enterSection(Section::Extension);
	}else if(szSection == TAG_ELEMENT){
		enterSection(Section::Element);
	}else if(szSection == TAG_PART){
		enterSection(Section::Part);
	}else if(szSection == TAG_INPUT){
		enterSection(Section::Input);
	}else if(szSection == TAG_OUTPUT){
		enterSection(Section::Output);
	}else if(szSection == TAG_MESSAGE){
		enterSection(Section::Message);
	}else if(szSection == TAG_OPERATION){
		enterSection(Section::Operation);
	}else if(szSection == TAG_RESTRICTION){
		enterSection(Section::Restriction);
	}else if(szSection == TAG_MAX_LENGTH){
		enterSection(Section::MaxLength);
	}else if(szSection == TAG_MIN_LENGTH){
		enterSection(Section::MinLength);
	}else if(szSection == TAG_ENUMERATION){
		enterSection(Section::Enumeration);
	}else if(szSection == TAG_LIST){
		enterSection(Section::List);
	}else if(szSection == TAG_UNION){
		enterSection(Section::Union);
	}else if(szSection == TAG_ANY){
		enterSection(Section::Any);
	}
	else{
		enterSection(Section::Other);
	}
}

void QWSDLParserHandler::enterSection(Section::Name section)
{
	m_sectionStack.push(section);
}

void QWSDLParserHandler::leaveSection()
{
	m_sectionStack.pop();
}

bool QWSDLParserHandler::inSection(Section::Name section) const
{
	return m_sectionStack.contains(section);
}

Section::Name QWSDLParserHandler::parentSection() const
{
	if(m_sectionStack.count() > 1){
		return m_sectionStack.at(m_sectionStack.count() - 2);
	}
	return Section::Other;
}

Section::Name QWSDLParserHandler::currentSection() const
{
	return m_sectionStack.top();
}
