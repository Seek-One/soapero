//
// Created by ebeuque on 29/06/2026.
//

#include "Utils/StringUtils.h"

#include "QtCppTargetEngine.h"

QtCppTargetEngine::QtCppTargetEngine()
{

}

QtCppTargetEngine::~QtCppTargetEngine()
{

}


void QtCppTargetEngine::doWriteNamespaceTargetInfos(QTextStream& os, const QString& szPrefix, const QString& szNamespace, const QString& szNamespaceURI) const
{
	os << CRLF;
	os << "const QString " << szPrefix << "_TargetNamespace = \"" << szNamespace << "\";" CRLF;
	os << "const QString " << szPrefix << "_TargetNamespaceUri = \"" << szNamespaceURI << "\";" CRLF;
	os << CRLF;
}

//////////////////
// Service files
//////////////////

void QtCppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QStringList list;
	bool bSoapEnvelopeFaultIncluded = false;

	os << "#include <QString>" << CRLF;
	os << "#include <QMap>" << CRLF;
	os << "#include <QDomDocument>" << CRLF;
	os << CRLF;

	os << "#include \"Service.h\"" << CRLF << CRLF;

	QString szTmpFileName;

	MessageSharedPtr pMessage;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {

		OperationSharedPtr pOperation = *operation;
		if(!pOperation->getInputMessage()) {
			continue;
		}
		//qDebug(qPrintable(pOperation->getName()));
		//qDebug(qPrintable(pOperation->getInputMessage() ? "NotNull" : "Null"));
		//qDebug(qPrintable(pOperation->getInputMessage()->getLocalName()));
		//qDebug(qPrintable(pOperation->getInputMessage()->getParameter() ? "NotNull" : "Null"));

		if(!bSoapEnvelopeFaultIncluded && pOperation->getSoapEnvelopeFaultType()){
			os << "#include \"" << getComplexTypeHeaderPath((*operation)->getSoapEnvelopeFaultType(), FileCategory_Service) << "\"" << CRLF;
			bSoapEnvelopeFaultIncluded = true;
		}

		pMessage = pOperation->getInputMessage();
		if(pMessage && pMessage->getParameter()){
			szTmpFileName = getHeaderFilePath(pMessage->getParameter(), FileCategory_Service);
			if(!list.contains(szTmpFileName)) {
				os << "#include \"" << szTmpFileName << "\"" << CRLF;
				list.append(szTmpFileName);
			}
		}

		pMessage = pOperation->getOutputMessage();
		if(pMessage && pMessage->getParameter()){
			szTmpFileName = getHeaderFilePath(pMessage->getParameter(), FileCategory_Service);
			if(!list.contains(szTmpFileName)) {
				os << "#include \"" << szTmpFileName << "\"" << CRLF;
				list.append(szTmpFileName);
			}
		}
	}

	os << CRLF;
	os << CRLF;
}

void QtCppTargetEngine::doWriteHeaderClass(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QString szClassname = pService->getName();

	// Start class
	doWriteHeaderClassStart(os, szClassname, "Service");
	// Constructor/destructor declaration
	doWriteHeaderClassInitializers(os, szClassname);

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		if(!(*operation)->getInputMessage()) {
			continue;
		}
		os << "\t" << (*operation)->getOperationDeclaration() << CRLF;
	}

	os << CRLF;
	os << "\tQMap<QString, QString> buildNamespaceRoutingMap(const QDomDocument& doc) const;" << CRLF;

	os << CRLF;
	os << "public:" << CRLF;
	os << "\tvoid setDebug(bool bDebug);" << CRLF;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\tbool m_bDebug;" << CRLF;

	os << "};" << CRLF;
}

void QtCppTargetEngine::doWriteCppIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{
	bool bHasIncludes = false;
	bool bQStringListIncluded = false;

	if(!pType){
		return;
	}

	if(pType->getTypeMode() == Type::TypeComplex)
	{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(pComplexType->getAttributeList()){
			if(pComplexType->getAttributeList()->containsListAttribute()){
				os << "#include <QStringList>" << CRLF;
				bQStringListIncluded = true;
			}
		}
		if(!bQStringListIncluded && pComplexType->getExtensionType() && pComplexType->isExtensionTypeList())
		{
			os << "#include <QStringList>" << CRLF;
			bQStringListIncluded = true;
		}

		bHasIncludes = bQStringListIncluded;

		if(pComplexType->getElementList())
		{
			ElementList::const_iterator iter;
			for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
				if((*iter)->getType() && (*iter)->isPointer()){
					os << "#include \"" << getHeaderFilePath((*iter)->getType(), FileCategory_Type) << "\"" << CRLF;
					bHasIncludes = true;
				}
			}
		}

		if(bHasIncludes){
			os << CRLF;
		}
	}
}

void QtCppTargetEngine::doWriteCppClass(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QString szClassname = pService->getName();

	os << szClassname << "::" << szClassname << "() {" << CRLF;
	os << "\tm_bDebug = false;" << CRLF;
	os << "}" << CRLF;
	os << CRLF;
	os << szClassname << "::~" << szClassname << "() {}" << CRLF;
	os << CRLF;
	os << "void " << szClassname << "::setDebug(bool bDebug) {" << CRLF;
	os << "\tm_bDebug = bDebug;" << CRLF;
	os << "}" << CRLF;
	os << CRLF;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		if(!(*operation)->getInputMessage()) {
			continue;
		}
		os << (*operation)->getOperationDefinition(szClassname, m_szNamespace) << CRLF;
	}

	os << "QMap<QString, QString> " << szClassname << "::buildNamespaceRoutingMap(const QDomDocument& doc) const" CRLF;
	os << "{" CRLF;
	os << "\tQMap<QString, QString> map;" CRLF;
	os << "\tQString szSoapEnvNamespace = DEFAULT_SOAP_ENV_NAMESPACE;" CRLF;
	os << CRLF;
	os << "\t// Try to find the real soap envelope namespace" CRLF;
	os << "\tQDomNode node = doc.documentElement();" CRLF;
	os << "\twhile(!node.isNull()){" CRLF;
	os << "\t\tQDomElement elem = node.toElement();" CRLF;
	os << "\t\tif(!elem.isNull() && elem.tagName().endsWith(\":Envelope\")){" CRLF;
	os << "\t\t\tszSoapEnvNamespace = elem.tagName().split(\":\")[0];" CRLF;
	os << "\t\t}" CRLF;
	os << "\t\tnode = node.nextSibling();" CRLF;
	os << "\t}" CRLF;
	os << CRLF;
	os << "\tif(doc.elementsByTagName(szSoapEnvNamespace + \":Envelope\").size() > 0){" << CRLF;
	os << "\t\tQDomElement root = doc.elementsByTagName(szSoapEnvNamespace + \":Envelope\").at(0).toElement();" << CRLF;
	os << "\t\tint iNbAttributes = root.attributes().size();" << CRLF;
	os << "\t\tfor(int i = 0; i < iNbAttributes; ++i){" << CRLF;
	os << "\t\t\tQDomAttr attr = root.attributes().item(i).toAttr();" << CRLF;
	os << "\t\t\tQStringList nameList = attr.name().split(\":\");" << CRLF;
	os << "\t\t\tmap.insert(attr.value(), nameList[1]);" << CRLF;
	os << "\t\t}" CRLF;
	os << "\t}" CRLF;
	os << "\treturn map;" CRLF;
	os << "}" CRLF;
}

//////////////////////////////////
// Request response elements files
//////////////////////////////////

void QtCppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pRequestResponseElement) const
{
	TypeSharedPtr pType = pRequestResponseElement->getType();

	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}

	os << "#include <QDomElement>" << CRLF;
	os << "#include <QList>" << CRLF;
	os << "#include <QString>" << CRLF;
	os << CRLF;

	AttributeListSharedPtr pListAttributes;
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements;
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(pComplexType){
		pListAttributes = pComplexType->getAttributeList();
		pListElements = pComplexType->getElementList();

		if(!pComplexType->getExtensionType().isNull()) {
			os << "#include \"" << getHeaderFilePath(pComplexType->getExtensionType(), FileCategory_Type) << "\"" << CRLF;
			os << CRLF;
		}
	}

	if((pListAttributes && pListAttributes->count() > 0) || (pListElements && pListElements->count() > 0))
	{
		QStringList list;

		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				continue;
			}

			// Add required include file
			QString szFilename = getHeaderFilePath(pAttribute->getType(), FileCategory_Type);
			if(!list.contains(szFilename)) {
				os << "#include \"" << szFilename << "\"" << CRLF;
				list.append(szFilename);
			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				continue;
			}

			// Add required include file
			QString szFilename = getHeaderFilePath(pElement->getType(), FileCategory_Message);
			if(!list.contains(szFilename)) {
				os << "#include \"" << szFilename << "\"" << CRLF;
				list.append(szFilename);
			}
		}
	}
}

void QtCppTargetEngine::doWriteHeaderClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassName =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName(true);
	TypeSharedPtr pType = pElement->getType();
	QString szNamespace = StringUtils::secureString(pElement->getNamespace().toUpper());

	// Namespaces start
	doWriteNamespaceStart(os, szNamespace);
	doWriteNamespaceStart(os, "MSG");

	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}
	if(!pComplexType.isNull())
	{
		// Write namespace infos
		doWriteNamespaceTargetInfos(os, szClassName, pElement->getNamespace(), pElement->getNamespaceUri());

		startCppClass(os, szClassName, pComplexType);
		doWriteHeaderClassContent(os, pComplexType);
		endCppClass(os, szClassName);
	}

	// Namespaces end
	doWriteNamespaceEnd(os, "MSG");
	doWriteNamespaceEnd(os, szNamespace);
}

void QtCppTargetEngine::doWriteCppClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName();
	TypeSharedPtr pType = pElement->getType();
	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}
	if(pComplexType){
		pComplexType = pComplexType->clone();
		pComplexType->setLocalName(pElement->getLocalName());
		pComplexType->setNamespace(pElement->getNamespace());
	}
	QString szNamespace = StringUtils::secureString(pElement->getNamespace().toUpper());

	// Namespaces start
	doWriteNamespaceStart(os, szNamespace);
	doWriteNamespaceStart(os, "MSG");

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		doWriteCppClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	// Namespaces end
	doWriteNamespaceEnd(os, "MSG");
	doWriteNamespaceEnd(os, szNamespace);
}

//////////////////////////////////
// Types files
//////////////////////////////////

void QtCppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{
	os << "#include <QDomElement>" << CRLF;
	os << "#include <QList>" << CRLF;
	os << "#include <QString>" << CRLF;
	os << CRLF;

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(pSimpleType->isUnion()) {
			const auto& listUnionTypes = pSimpleType->getUnionTypes();
			for(const auto& pUnionTypeRef : listUnionTypes) {
				const auto& pUnionType = findTypeByName(pUnionTypeRef->getTypeName(), pUnionTypeRef->getNamespace());
				if (pUnionType) {
					os << "#include \"" << getHeaderFilePath(pUnionType, FileCategory_Type) << "\"" << CRLF;
				}else {
					qWarning("[Builder] Union type not found: %s:%s", qPrintable(pUnionTypeRef->getNamespace()), qPrintable(pUnionTypeRef->getTypeName()));
				}
			}
		}
		if(pSimpleType->hasVariableType()) {
			os << "#include \"" << getHeaderFilePath(pSimpleType, FileCategory_Type) << "\"" << CRLF;
		}
		os << CRLF;
	}else if(pType->getTypeMode() == Type::TypeComplex){

		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
		AttributeList::const_iterator attr;
		AttributeSharedPtr pAttribute;

		ElementListSharedPtr pListElements = pComplexType->getElementList();
		ElementList::const_iterator element;
		ElementSharedPtr pElement;

		if(!pComplexType->getExtensionType().isNull()) {
			os << "#include \"" << getHeaderFilePath(pComplexType->getExtensionType(), FileCategory_Type) << "\"" << CRLF;
		}

		if(pListAttributes->count() > 0 || pListElements->count() > 0) {

			QStringList list;

			for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
				if((*attr)->hasRef()){
					pAttribute = (*attr)->getRef();
				}else{
					pAttribute = *attr;
				}

				if(!pAttribute->getType()) {
					continue;
				}

				// Add required include file
				QString szFilename = getHeaderFilePath(pAttribute->getType(), FileCategory_Type);
				if(!list.contains(szFilename)) {
					os << "#include \"" << szFilename << "\"" << CRLF;
					list.append(szFilename);
				}
			}
			for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
				if((*element)->hasRef()){
					pElement = (*element)->getRef();
				}else{
					pElement = *element;
				}

				if(!pElement->getType() || pElement->isNested()){
					continue;
				}

				if(pElement->isPointer()){
					continue;
				}


				// Add required include file
				QString szFilename = getHeaderFilePath(pElement->getType(), FileCategory_Type);
				if(!szFilename.isEmpty() && !list.contains(szFilename)) {
					os << "#include \"" << szFilename << "\"" << CRLF;
					list.append(szFilename);
				}
			}
		}
		os << CRLF;
	}
}

void QtCppTargetEngine::doWriteHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	// Namespaces start
	doWriteNamespaceStart(os, szNamespace);
	doWriteNamespaceStart(os, "TYPES");

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		// Write namespace infos
		doWriteNamespaceTargetInfos(os, pSimpleType->getLocalName(true), pSimpleType->getNamespace(), pSimpleType->getNamespaceUri());

		// Start class
		doWriteHeaderClassStart(os, szClassname);

		if(pSimpleType->isEnumeration()) {
			os << "public:" << CRLF;
			os << "\t" << pSimpleType->getEnumerationDeclaration() << CRLF;
			os << CRLF;
		}

		// Constructor/destructor declaration
		doWriteHeaderClassInitializers(os, szClassname);

		doWriteHeaderClassContent(os, pSimpleType);

		// End class
		doWriteHeaderClassEnd(os, szClassname);

	} else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		// Write namespace infos
		doWriteNamespaceTargetInfos(os, pComplexType->getLocalName(true), pComplexType->getNamespace(), pComplexType->getNamespaceUri());

		if(pComplexType->getElementList()){
			ElementList::const_iterator iter;
			for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
				if((*iter)->getType() && (*iter)->isPointer()){
					os << "class " << (*iter)->getType()->getLocalName(true) << ";" << CRLF << CRLF;
				}
			}
		}

		startCppClass(os, szClassname, pComplexType);
		doWriteHeaderClassContent(os, pComplexType);
		endCppClass(os, szClassname);
	}

	// Namespaces end
	doWriteNamespaceEnd(os, "TYPES");
	doWriteNamespaceEnd(os, szNamespace);
}

void QtCppTargetEngine::doWriteCppClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	// Namespaces start
	doWriteNamespaceStart(os, szNamespace);
	doWriteNamespaceStart(os, "TYPES");

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		// Constructor
		if(pSimpleType->isEnumeration()) {
			os << szClassname << "::" << szClassname << "()" << CRLF;
			os << "{" << CRLF;
			os << "\t" << pSimpleType->getVariableName() << " = Unknown;" << CRLF;
			os << "}" << CRLF;
			os << CRLF;
		}else{
			os << szClassname << "::" << szClassname << "() {}" << CRLF;
		}
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		doWriteCppClassContent(os, pSimpleType);

		os << CRLF;

	} else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		if(pComplexType->getExtensionType().isNull()) {
			os << szClassname << "::" << szClassname << "() {";
			if(pComplexType->getElementList()){
				ElementList::const_iterator iter;
				for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
					if((*iter)->getMaxOccurs() == 1){
						if((*iter)->getType() && ((*iter)->isNested() || (*iter)->isPointer())){
							os << CRLF << "\t" << (*iter)->getVariableName() << " = NULL;" << CRLF;
						}
					}
				}
			}
			os << "}" << CRLF;

			os << szClassname << "::~" << szClassname << "() {";
			if(pComplexType->getElementList()){
				ElementList::const_iterator iter;
				for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
					if((*iter)->getMaxOccurs() == 1){
						if((*iter)->getType() && ((*iter)->isNested() || (*iter)->isPointer())){
							os << CRLF << "\tif(" << (*iter)->getVariableName() << "){" << CRLF;
							os << "\t\tdelete " << (*iter)->getVariableName() << ";" << CRLF;
							os << "\t\t" << (*iter)->getVariableName() << " = NULL;" << CRLF;
							os << "\t}" << CRLF;
						}
					}
				}
			}
			os << "}" << CRLF;
			os << CRLF;
		}else{
			QString szExtendedClassName = getBaseClassName(m_szPrefix, pComplexType);
			os << szClassname << "::" << szClassname << "() : " << szExtendedClassName << "() {}" << CRLF;
			os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		}
		os << CRLF;

		doWriteCppClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	// Namespaces end
	doWriteNamespaceEnd(os, "TYPES");
	doWriteNamespaceEnd(os, szNamespace);
}

void QtCppTargetEngine::doWriteHeaderClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	//if(pSimpleType->isUnion()) {
	//	os << pSimpleType->getGetterSetterDeclarationForUnion();
	//}else {
		os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
		os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
	//}
	os << "\t" << pSimpleType->getSerializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getDeserializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getEnumConvertDeclaration() << CRLF;
	os << "\t" << pSimpleType->getIsNullDeclaration() << CRLF;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\t" << pSimpleType->getVariableDeclaration() << CRLF;
}

void QtCppTargetEngine::doWriteCppClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pSimpleType->getLocalName();
	szClassname = StringUtils::secureString(szClassname);

	os << pSimpleType->getSetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getGetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getSerializerDefinition(szClassname) << CRLF;
	os << pSimpleType->getDeserializerDefinition(szClassname) << CRLF;
	os << pSimpleType->getEnumConvertDefinition(szClassname) << CRLF;
	os << pSimpleType->getIsNullDefinition(szClassname) << CRLF;
	os << CRLF;
}

void QtCppTargetEngine::doWriteHeaderClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		os << "public:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				qWarning("[Builder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << "\t" << pAttribute->getSetterDeclaration();
			os << "\t" << pAttribute->getGetterDeclaration();
			os << CRLF;
		}

		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				qWarning("[Builder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << "\t" << pElement->getSetterDeclaration() << CRLF ;
			os << "\t" << pElement->getGetterDeclaration() << CRLF ;
			os << CRLF;

		}
	}

	os << "\t" << pComplexType->getSerializerDeclaration() << CRLF;
	os << "\t" << pComplexType->getDeserializerDeclaration() << CRLF;
	os << CRLF;
	os << "\t" << pComplexType->getIsNullDeclaration() << CRLF;
	os << CRLF;
	os << "\t" << pComplexType->getGetNamespaceDeclarationDeclaration() << CRLF;
	os << CRLF;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {
		os << "private:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				continue;
			}

			os << "\t" << pAttribute->getVariableDeclaration() << CRLF;
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				continue;
			}

			os << "\t" << pElement->getVariableDeclaration() << CRLF ;
		}
	}
}

void QtCppTargetEngine::doWriteCppClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pComplexType->getLocalName();

	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				qWarning("[Builder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << pAttribute->getSetterDefinition(szClassname);
			os << pAttribute->getGetterDefinition(szClassname);
			os << CRLF;
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			ComplexTypeSharedPtr pType = qSharedPointerCast<ComplexType>(pElement->getType());

			if(!pElement->getType()) {
				qWarning("[Builder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << pElement->getSetterDefinition(szClassname) << CRLF ;
			os << pElement->getGetterDefinition(szClassname) << CRLF ;
			os << CRLF;
		}
	}
	os << pComplexType->getSerializerDefinition(szClassname, szTargetNamespace) << CRLF;
	os << pComplexType->getDeserializerDefinition(szClassname) << CRLF;
	os << CRLF;
	os << pComplexType->getIsNullDefinition(szClassname) << CRLF;
	os << pComplexType->getGetNamespaceDeclarationDefinition(szClassname) << CRLF;
}

QString QtCppTargetEngine::getBaseClassName(const QString& szPrefix, const ComplexTypeSharedPtr& pComplexType)
{
	QString szExtendedClassName;
	if(!pComplexType->getExtensionType().isNull())
	{
		if(pComplexType->isExtensionTypeList()){
			szExtendedClassName = "QList<";
		}
		if(pComplexType->getExtensionType()->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
			szExtendedClassName += pSimpleType->getCPPTypeNameString();
		}else{
			QString szExtensionName = pComplexType->getExtensionType()->getNameWithNamespace();
			szExtendedClassName += (!szPrefix.isEmpty() ? szPrefix : "") + szExtensionName;
		}
		if(pComplexType->isExtensionTypeList()){
			szExtendedClassName += ">";
		}
	}
	return szExtendedClassName;
}

void QtCppTargetEngine::startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const
{
	QString szExtendedClassName = getBaseClassName(m_szPrefix, pComplexType);

	// Start class
	doWriteHeaderClassStart(os, szClassName, szExtendedClassName);
	// Constructor/destructor declaration
	doWriteHeaderClassInitializers(os, szClassName);
}

void QtCppTargetEngine::endCppClass(QTextStream& os, const QString& szClassName) const
{
	doWriteHeaderClassEnd(os, szClassName);
}