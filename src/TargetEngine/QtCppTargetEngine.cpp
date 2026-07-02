//
// Created by ebeuque on 29/06/2026.
//

#include "Model/Type.h"
#include "Model/Element.h"
#include "Model/Attribute.h"

#include "Utils/StringUtils.h"
#include "Utils/ModelUtils.h"

#include "LanguageWriter/QtCppWriter.h"

#include "QtCppTargetEngine.h"

QtCppTargetEngine::QtCppTargetEngine()
{

}

QtCppTargetEngine::~QtCppTargetEngine()
{

}

void QtCppTargetEngine::doWriteClassInfos(QTextStream& os, const QString& szPrefix, const QString& szSchemaURI, const QString& szNamespace, const QString& szNamespaceURI) const
{
	os << CRLF;
	if (!szSchemaURI.isEmpty()) {
		os << "const QString " << szPrefix << "_SchemaUri = \"" << szSchemaURI << "\";" CRLF;
	}
	os << "const QString " << szPrefix << "_TargetNamespace = \"" << szNamespace << "\";" CRLF;
	os << "const QString " << szPrefix << "_TargetNamespaceUri = \"" << szNamespaceURI << "\";" CRLF;
	os << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const
{
	QString szDeclaration;
	if (iParamMode == SetterParamModePointer) {
		szDeclaration = "void set%0List(const QList<%1*>& %2List);";
	}else if (iParamMode == SetterParamModeConst2) {
		szDeclaration = "void set%0List(const QList<%1>& %2);";
	}else{
		szDeclaration = "void set%0List(const QList<%1>& %2List);";
	}
	szDeclaration = szDeclaration.arg(szFuncName).arg(szParamType).arg(szParamName);
	os << "\t" << szDeclaration << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationGetterList(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const
{
	QString szDeclaration;
	if (iReturnMode == GetterReturnModePointer) {
		szDeclaration += "const QList<%0*>& get%1List() const;";
	}else{
		szDeclaration = "const QList<%0>& get%1List() const;";
	}
	szDeclaration = szDeclaration.arg(szMemberType).arg(szFuncName);
	os << "\t" << szDeclaration << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionSetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const
{
	QString szDefinition;
	if (iParamMode == SetterParamModePointer) {
		szDefinition += "void %0::set%1List(const QList<%2*>& %3List)" CRLF;
	}else if (iParamMode == SetterParamModeConst2) {
		szDefinition = "void %0::set%1List(const QList<%2>& %3)" CRLF;
	}else{
		szDefinition += "void %0::set%1List(const QList<%2>& %3List)" CRLF;
	}
	szDefinition += "{" CRLF;
	if (iParamMode == SetterParamModeConst2) {
		szDefinition += "\t%4 = %3;" CRLF;
	}else {
		szDefinition += "\t%4 = %3List;" CRLF;
	}
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(szParamType).arg(szParamName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionAddList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const
{
	QString szDefinition;
	if (iParamMode == SetterParamModePointer) {
		szDefinition += "void %0::add%1(%2* %3)" CRLF;
	}else {
		szDefinition += "void %0::add%1(const %2& %3)" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\t%4.append(%3);" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(szParamType).arg(szParamName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionGetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const
{
	QString szDefinition;
	if (iReturnMode == GetterReturnModePointer) {
		szDefinition += "const QList<%0*>& %1::get%2List() const" CRLF;
	}else{
		szDefinition += "const QList<%0>& %1::get%2List() const" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\treturn %3;" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szMemberType).arg(szClassName).arg(szFuncName).arg(szMemberName);
	os << szDefinition << CRLF;
}

//////////////////
// Service files
//////////////////

void QtCppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QStringList list;
	bool bSoapEnvelopeFaultIncluded = false;

	QtCppWriter langWriter(os);
	// Includes
	langWriter.writeIncludeFileSystem("QString");
	langWriter.writeIncludeFileSystem("QMap");
	langWriter.writeIncludeFileSystem("QDomDocument");
	os << CRLF;
	langWriter.writeIncludeFileLocal("Service.h");
	os << CRLF;

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
			const auto& szHeaderFilePath = getComplexTypeHeaderPath((*operation)->getSoapEnvelopeFaultType(), FileCategory_Service);
			langWriter.writeIncludeFileLocal(szHeaderFilePath);
			bSoapEnvelopeFaultIncluded = true;
		}

		pMessage = pOperation->getInputMessage();
		if(pMessage && pMessage->getParameter()){
			szTmpFileName = getHeaderFilePath(pMessage->getParameter(), FileCategory_Service);
			if(!list.contains(szTmpFileName)) {
				list.append(szTmpFileName);
				langWriter.writeIncludeFileLocal(szTmpFileName);
			}
		}

		pMessage = pOperation->getOutputMessage();
		if(pMessage && pMessage->getParameter()){
			szTmpFileName = getHeaderFilePath(pMessage->getParameter(), FileCategory_Service);
			if(!list.contains(szTmpFileName)) {
				list.append(szTmpFileName);
				langWriter.writeIncludeFileLocal(szTmpFileName);
			}
		}
	}

	os << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationClass(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QString szClassname = pService->getName();

	// Start class
	//os << CRLF;
	doWriteDeclarationClassStart(os, szClassname, "Service");
	// Constructor/destructor declaration
	doWriteDeclarationClassInitializers(os, szClassname, false);

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		if(!(*operation)->getInputMessage()) {
			continue;
		}
		doWriteDeclarationOperation(os, *operation);
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

void QtCppTargetEngine::doWriteDefinitionIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{
	bool bHasIncludes = false;
	bool bQStringListIncluded = false;

	if(!pType){
		return;
	}

	QtCppWriter langWriter(os);

	if(pType->getTypeMode() == Type::TypeComplex)
	{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(pComplexType->getAttributeList()){
			if(pComplexType->getAttributeList()->containsListAttribute()){
				langWriter.writeIncludeFileSystem("QStringList");
				bQStringListIncluded = true;
			}
		}
		if(!bQStringListIncluded && pComplexType->getExtensionType() && pComplexType->isExtensionTypeList())
		{
			langWriter.writeIncludeFileSystem("QStringList");
			bQStringListIncluded = true;
		}

		bHasIncludes = bQStringListIncluded;

		if(pComplexType->getElementList())
		{
			ElementList::const_iterator iter;
			for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
				if((*iter)->getType() && (*iter)->isPointer()){
					const auto& szHeaderFilePath = getHeaderFilePath((*iter)->getType(), FileCategory_Type);
					langWriter.writeIncludeFileLocal(szHeaderFilePath);
					bHasIncludes = true;
				}
			}
		}

		if(bHasIncludes){
			os << CRLF;
		}
	}
}

void QtCppTargetEngine::doWriteDefinitionClass(QTextStream& os, const ServiceSharedPtr& pService) const
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
		doWriteDefinitionOperation(os, *operation, szClassname, m_szNamespace);
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

void QtCppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pRequestResponseElement) const
{
	TypeSharedPtr pType = pRequestResponseElement->getType();

	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}

	QtCppWriter langWriter(os);
	// Includes
	langWriter.writeIncludeFileSystem("QDomElement");
	langWriter.writeIncludeFileSystem("QList");
	langWriter.writeIncludeFileSystem("QString");
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
			const auto& szHeaderFilePath = getHeaderFilePath(pComplexType->getExtensionType(), FileCategory_Type);
			langWriter.writeIncludeFileLocal(szHeaderFilePath);
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
				langWriter.writeIncludeFileLocal(szFilename);
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
				langWriter.writeIncludeFileLocal(szFilename);
				list.append(szFilename);
			}
		}
	}
	os << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassName =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName(true);
	TypeSharedPtr pType = pElement->getType();
	QString szNamespace = StringUtils::secureString(pElement->getNamespace().toUpper());
	
	QtCppWriter langWriter(os);
	
	// Namespaces start
	langWriter.writeNamespaceStart(szNamespace);
	langWriter.writeNamespaceStart("MSG");

	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}
	if(!pComplexType.isNull())
	{
		// Write namespace infos
		doWriteClassInfos(os, szClassName, pElement->getSchemaUri(), pElement->getNamespace(), pElement->getNamespaceUri());

		startCppClass(os, szClassName, pComplexType);
		doWriteDeclarationClassContent(os, pComplexType);
		endCppClass(os, szClassName);
	}else {
		os << CRLF;
	}

	// Namespaces end
	langWriter.writeNamespaceEnd("MSG");
	langWriter.writeNamespaceEnd(szNamespace);
}

void QtCppTargetEngine::doWriteDefinitionClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
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

	QtCppWriter langWriter(os);

	// Namespaces start
	langWriter.writeNamespaceStart(szNamespace);
	langWriter.writeNamespaceStart("MSG");
	os << CRLF;

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		doWriteDefinitionClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	// Namespaces end
	langWriter.writeNamespaceEnd("MSG");
	langWriter.writeNamespaceEnd(szNamespace);
}

//////////////////////////////////
// Types files
//////////////////////////////////

void QtCppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{
	QtCppWriter langWriter(os);
	// Includes
	langWriter.writeIncludeFileSystem("QDomElement");
	langWriter.writeIncludeFileSystem("QList");
	langWriter.writeIncludeFileSystem("QString");
	os << CRLF;

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(pSimpleType->isUnion()) {
			const auto& listUnionTypes = pSimpleType->getUnionTypes();
			for(const auto& pUnionTypeRef : listUnionTypes) {
				const auto& pUnionType = findTypeByName(pUnionTypeRef->getTypeName(), pUnionTypeRef->getNamespace());
				if (pUnionType) {
					const auto& szHeaderFilePath =  getHeaderFilePath(pUnionType, FileCategory_Type);
					langWriter.writeIncludeFileLocal(szHeaderFilePath);
				}else {
					qWarning("[Builder] Union type not found: %s:%s", qPrintable(pUnionTypeRef->getNamespace()), qPrintable(pUnionTypeRef->getTypeName()));
				}
			}
		}
		if(pSimpleType->hasVariableType()) {
			const auto& szHeaderFilePath =  getHeaderFilePath(pSimpleType, FileCategory_Type);
			langWriter.writeIncludeFileLocal(szHeaderFilePath);
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
			const auto& szHeaderFilePath =  getHeaderFilePath(pComplexType->getExtensionType(), FileCategory_Type);
			langWriter.writeIncludeFileLocal(szHeaderFilePath);
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
					list.append(szFilename);
					langWriter.writeIncludeFileLocal(szFilename);
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
					list.append(szFilename);
					langWriter.writeIncludeFileLocal(szFilename);
				}
			}
		}
		os << CRLF;
	}
}

void QtCppTargetEngine::doWriteDeclarationClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	QtCppWriter langWriter(os);

	// Namespaces start
	langWriter.writeNamespaceStart(szNamespace);
	langWriter.writeNamespaceStart("TYPES");

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		// Write namespace infos
		doWriteClassInfos(os, pSimpleType->getLocalName(true), pSimpleType->getSchemaUri(), pSimpleType->getNamespace(), pSimpleType->getNamespaceUri());

		// Start class
		doWriteDeclarationClassStart(os, szClassname);

		if(pSimpleType->isEnumeration()) {
			os << "public:" << CRLF;
			doWriteDeclarationEnumeration(os, pSimpleType);
			os << CRLF;
		}

		// Constructor/destructor declaration
		doWriteDeclarationClassInitializers(os, szClassname, pSimpleType->isEnumeration());

		doWriteDeclarationClassContent(os, pSimpleType);

		// End class
		doWriteDeclarationClassEnd(os, szClassname);

	} else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		// Write namespace infos
		doWriteClassInfos(os, pComplexType->getLocalName(true), pComplexType->getSchemaUri(), pComplexType->getNamespace(), pComplexType->getNamespaceUri());

		if(pComplexType->getElementList()){
			ElementList::const_iterator iter;
			for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
				if((*iter)->getType() && (*iter)->isPointer()){
					os << "class " << (*iter)->getType()->getLocalName(true) << ";" << CRLF << CRLF;
				}
			}
		}

		startCppClass(os, szClassname, pComplexType);
		doWriteDeclarationClassContent(os, pComplexType);
		endCppClass(os, szClassname);
	}

	// Namespaces end
	langWriter.writeNamespaceEnd("TYPES ");
	langWriter.writeNamespaceEnd(szNamespace);
}

void QtCppTargetEngine::doWriteDeclarationSerializer(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szDeclaration;
	if (pType->getTypeMode() == Type::TypeSimple) {
		szDeclaration = "QString serialize() const;";
	}else if (pType->getTypeMode() == Type::TypeComplex) {
		szDeclaration = "QString serialize(bool bOnlyContent = false) const;";
	}
	if (!szDeclaration.isEmpty()) {
		os << "\t" << szDeclaration << CRLF;
	}
}

void QtCppTargetEngine::doWriteDeclarationDeserializer(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szDeclaration;
	if (pType->getTypeMode() == Type::TypeSimple) {
		szDeclaration = "void deserialize(const QDomElement& element);";
	}else if (pType->getTypeMode() == Type::TypeComplex) {
		szDeclaration = "void deserialize(QDomElement& element);";
	}
	if (!szDeclaration.isEmpty()) {
		os << "\t" << szDeclaration << CRLF;
	}

	if (pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(pSimpleType->isEnumeration()){
			szDeclaration = "void deserialize(const QDomAttr& attr);";
			os << "\t" << szDeclaration << CRLF;
		}
	}
}

void QtCppTargetEngine::doWriteDefinitionClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	QtCppWriter langWriter(os);

	// Namespaces start
	langWriter.writeNamespaceStart(szNamespace);
	langWriter.writeNamespaceStart("TYPES");
	os << CRLF;

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

		doWriteDefinitionClassContent(os, pSimpleType);

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

		doWriteDefinitionClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	// Namespaces end
	langWriter.writeNamespaceEnd("TYPES");
	langWriter.writeNamespaceEnd(szNamespace);
}

void QtCppTargetEngine::doWriteDeclarationClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	// Getter/setter
	doWriteDeclarationGetterSetter(os, pSimpleType);
	// Serialization
	doWriteDeclarationSerializer(os, pSimpleType);
	doWriteDeclarationDeserializer(os, pSimpleType);
	// Enum conversion
	doWriteDeclarationEnumConvert(os, pSimpleType);
	// Null declaration
	doWriteDeclarationIsNull(os, pSimpleType);
	os << CRLF;
	// Variables
	os << "private:" << CRLF;
	doWriteDeclarationVariable(os, pSimpleType);
}

void QtCppTargetEngine::doWriteDeclarationGetterSetter(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szFuncName = ModelUtils::getCapitalizedName(pSimpleType->getLocalName());
	QString szParamType = pSimpleType->getCPPTypeNameValuesString();
	QString szParamName = ModelUtils::getUncapitalizedName(pSimpleType->getLocalName());

	/*
	//if(pSimpleType->isUnion()) {
		QString szDeclaration;

		const auto& listTypeUnion = getUnionTypes();
		for(const auto& pTypeUnionRef : listTypeUnion)
		{
			const auto& pTypeUnion = pTypeUnionRef;

			QString szNamespace = pTypeUnion->getNamespace().toUpper();
			QString szTypeName = pTypeUnion->getTypeName();
			QString szTypeDeclaration = szNamespace + "::" + szTypeName;

			szDeclaration += "\t" + QString("void set%0(const %1& %2);").arg(szTypeName).arg(szTypeDeclaration).arg(szTypeName) + CRLF;
			szDeclaration += "\t" + QString("const %0& get%1() const;").arg(szTypeDeclaration).arg(szTypeName) + CRLF;
			szDeclaration += CRLF;
		}
		return szDeclaration;;
	}
	}*/
	if (pSimpleType->isEnumeration()) {
		doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeDefault);
		doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeDefault);
	}else {
		doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
		doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
	}
}

void QtCppTargetEngine::doWriteDefinitionGetterSetter(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const
{
	QString szLocalName = pSimpleType->getLocalName();
	QString szFuncName = ModelUtils::getCapitalizedName(szLocalName);
	QString szParamType = pSimpleType->getCPPTypeNameValuesString();
	QString szParamName = ModelUtils::getUncapitalizedName(szLocalName);
	QString szMemberName = pSimpleType->getVariableName();

	if (pSimpleType->isEnumeration()) {
		doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeDefault);
		doWriteDefinitionGetter(os, szClassName, szFuncName, szParamType, szMemberName, GetterReturnModeDefault);
	}else {
		doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
		doWriteDefinitionGetter(os, szClassName, szFuncName, szParamType, szMemberName, GetterReturnModeConst);
	}
}

void QtCppTargetEngine::doWriteDefinitionSerializerDeserializer(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const
{
	QString szLocalName = pSimpleType->getLocalName();
	QString szFuncName = ModelUtils::getCapitalizedName(szLocalName);
	QString szMemberName = pSimpleType->getVariableName();

	QString szDefinition;

	if(pSimpleType->isEnumeration()) {
		// Serialize
		szDefinition = ""
			"QString %0::serialize() const" CRLF
			"{" CRLF
			"\tif(!isNull()) {" CRLF
			"\t\treturn get%1ToString();" CRLF
			"\t}" CRLF
			"\t return \"\";" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(szFuncName) << CRLF;

		// Deserialize
		szDefinition = ""
			"void %0::deserialize(const QDomElement& element)" CRLF
			"{" CRLF
			"\tset%1FromString(element.text().trimmed());" CRLF
			"}" CRLF CRLF;
		szDefinition += "void %0::deserialize(const QDomAttr& attr)" CRLF
			"{" CRLF
			"\tset%1FromString(attr.value().trimmed());" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(szFuncName) << CRLF;
	} else {
		// Serialize
		szDefinition = ""
			"QString %0::serialize() const" CRLF
			"{" CRLF
			"\tif(!isNull()) {" CRLF
			"\t\treturn \"<%1>\" + %2.serialize() + \"</%1>\";" CRLF
			"\t}" CRLF
			"\t return \"\";" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(szFuncName).arg(szMemberName) << CRLF;

		// Deserialize
		szDefinition = ""
			"void %0::deserialize(const QDomElement& element)" CRLF
			"{" CRLF
			"\tQDomElement child = element.firstChild().toElement();" CRLF
			"\twhile(!child.isNull()) {" CRLF
			"\t\tif((child.tagName() == \"%1\") || child.tagName().endsWith(\"%2\")) {" CRLF
			"\t\t%3.deserialize(child);" CRLF
			"\t\t}" CRLF
			"\tchild = child.nextSibling().toElement();" CRLF
			"\t}" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(pSimpleType->getTagQualifiedName()).arg(szFuncName).arg(szMemberName) << CRLF;
	}
}

void QtCppTargetEngine::doWriteDeclarationIsNull(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	os << "\tbool isNull() const;" << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionIsNull(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const
{
	if(pSimpleType->isEnumeration()) {
		QString szDefinition = ""
			"bool %0::isNull() const" CRLF
			"{" CRLF
			"\t return %1 == Unknown;" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(pSimpleType->getVariableName());
	} else {
		QString szDefinition = ""
			"bool %0::isNull() const" CRLF
			"{" CRLF
			"\t return %1.isNull();" CRLF
			"}" CRLF;
		os << szDefinition.arg(szClassName).arg(pSimpleType->getVariableName());
	}
	os << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationEnumConvert(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szFuncName = ModelUtils::getCapitalizedName(pSimpleType->getLocalName());
	QString szDeclaration;
	if(pSimpleType->isEnumeration()) {
		szDeclaration += "\tvoid set%0FromString(const QString& szValue);" CRLF;
		szDeclaration += "\tQString get%0ToString() const;";
		szDeclaration = szDeclaration.arg(szFuncName);
	}
	os << szDeclaration << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionEnumConvert(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const
{
	QString szDefinition;

	QString szFuncName = ModelUtils::getCapitalizedName(pSimpleType->getLocalName());

	if(pSimpleType->isEnumeration()) {

		const auto& listEnumerationValues = pSimpleType->getEnumerationValues();

		szDefinition += ""
		"void %0::set%1FromString(const QString& szValue)" CRLF
		"{" CRLF
		"\tif((szValue == \"" + listEnumerationValues[0] + "\") || szValue.endsWith(\":" + StringUtils::getLocalNameIfPossible(listEnumerationValues[0]) + "\")) {" CRLF
		"\t\t %2 = %0::" + StringUtils::removeNonAlphaNum(listEnumerationValues[0]) + ";" CRLF
		"\t}";

		for(int i=1; i < listEnumerationValues.count(); ++i) {
			szDefinition += ""
			"else if((szValue == \"" + listEnumerationValues[i] + "\") || szValue.endsWith(\":" + StringUtils::getLocalNameIfPossible(listEnumerationValues[i]) + "\")) {" CRLF
			"\t\t%2 = %0::" + StringUtils::removeNonAlphaNum(listEnumerationValues[i]) + ";" CRLF
			"\t}";
		}
		szDefinition += " else {" CRLF;
		szDefinition += "\t\t%2 = %0::Unknown;" CRLF;
		szDefinition += "\t}" CRLF;
		szDefinition += CRLF;
		szDefinition += "}" CRLF CRLF;

		szDefinition += ""
		"QString %0::get%1ToString() const" CRLF
		"{" CRLF
		"\tswitch(%2) {" CRLF;

		for(int i=0; i < listEnumerationValues.count(); ++i) {
			szDefinition += ""
			"\tcase " + StringUtils::removeNonAlphaNum(listEnumerationValues[i]) + ":" CRLF
			"\t\treturn \"" + listEnumerationValues[i] + "\";" CRLF;
		}
		szDefinition += "\tdefault:" CRLF;
		szDefinition += "\t\treturn \"Unknown\";" CRLF;
		szDefinition += "\t}" CRLF;
		szDefinition += "}" CRLF;

		szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(pSimpleType->getVariableName());
	}
	os << szDefinition << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationEnumeration(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	bool bHasUnknownValue = false;
	QString szDeclaration;

	const auto& bRestricted = pSimpleType->isRestricted();
	const auto& listEnumerationValues = pSimpleType->getEnumerationValues();

	if(bRestricted) {
		szDeclaration += "\tenum Values {";
		for(int i=0; i < listEnumerationValues.count(); ++i) {
			if(listEnumerationValues[i] == "Unknown") {
				bHasUnknownValue = true;
			}
			if(i==0) {
				szDeclaration += StringUtils::removeNonAlphaNum(listEnumerationValues[i]);
			} else {
				szDeclaration += ", " + StringUtils::removeNonAlphaNum(listEnumerationValues[i]);
			}
		}
		if(!bHasUnknownValue) {
			szDeclaration += ", Unknown";
		}
		szDeclaration += "};";
	}
	os << szDeclaration << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationVariable(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QtCppWriter langWriter(os);

	const auto& szTypeName = pSimpleType->getCPPTypeNameValuesString();
	const auto& szVariableName = pSimpleType->getVariableName();
	langWriter.writeDeclarationVariable(szTypeName, szVariableName);
}

void QtCppTargetEngine::doWriteDefinitionClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pSimpleType->getLocalName();
	szClassname = StringUtils::secureString(szClassname);
	doWriteDefinitionGetterSetter(os, pSimpleType, szClassname);
	doWriteDefinitionSerializerDeserializer(os, pSimpleType, szClassname);
	doWriteDefinitionEnumConvert(os, pSimpleType, szClassname);
	doWriteDefinitionIsNull(os, pSimpleType, szClassname);
	os << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
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
			doWriteDeclarationGetterSetter(os, pAttribute);
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

			doWriteDeclarationGetterSetter(os, pElement);
			os << CRLF;

		}
	}

	doWriteDeclarationSerializer(os, pComplexType);
	doWriteDeclarationDeserializer(os, pComplexType);
	os << CRLF;
	doWriteDeclarationIsNull(os, pComplexType);
	os << CRLF;
	doWriteDeclarationGetNamespaceDeclaration(os, pComplexType);
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

			doWriteDeclarationVariable(os, pAttribute);
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

			doWriteDeclarationVariable(os, pElement);
		}
	}
}

void QtCppTargetEngine::doWriteDefinitionClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace) const
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

			doWriteDefinitionGetterSetter(os, pAttribute, szClassname);
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

			doWriteDefinitionGetterSetter(os, pElement, szClassname);
			os << CRLF;
		}
	}

	doWriteDefinitionSerializer(os, pComplexType, szClassname, szTargetNamespace);
	doWriteDefinitionDeserializer(os, pComplexType, szClassname, szTargetNamespace);
	os << CRLF;
	doWriteDefinitionIsNull(os, pComplexType, szClassname);
	doWriteDefinitionGetNamespaceDeclaration(os, pComplexType, szClassname);
}

void QtCppTargetEngine::doWriteDefinitionSerializer(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName, const QString& szTargetNamespace) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szIterator;
	int iIteratorCount = 0;

	QString szDefinition;

	// Serialize
	szDefinition = ""
		"QString %0::serialize(bool bOnlyContent) const" CRLF
		"{" CRLF
		"\tQString szValue;" CRLF
		"\tif(!bOnlyContent) {" CRLF
		"\t\tszValue += \"<%1:%2\";" CRLF
		"\t}" CRLF;

	// Not found other cases so we supposed attribute is only a simpleType
	const auto& pListAttribute = pComplexType->getAttributeList();
	for(attr = pListAttribute->constBegin(); attr != pListAttribute->constEnd(); ++attr)
	{
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if(pAttribute->getType()){
			if(pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
				if(pAttribute->isList()){
					szIterator = "iter_" + QString::number(iIteratorCount++);
					szDefinition += "\tif(_" + pAttribute->getName() + "List" + ".size() > 0) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\";" CRLF;
					szDefinition += QString("\t\tQList<%0>::const_iterator %1;" CRLF).arg( pSimpleType->getCPPTypeNameString()).arg(szIterator);
					szDefinition += QString("\t\tfor(%0 = _%1List.constBegin(); %0 != _%1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pAttribute->getName());
					szDefinition += QString("\t\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t\tszValue += %0->serialize() + \" \";" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t}" CRLF);
					szDefinition += "\t\t}" CRLF;
					szDefinition += "\t\tszValue +=\"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}else{
					szDefinition += "\tif(!" + pSimpleType->getVariableName() + ".isNull()) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\" + " + pSimpleType->getVariableName() + ".serialize() + \"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}
			}else if(pAttribute->getType()->getTypeMode() == Type::TypeComplex){
				ComplexTypeSharedPtr pComplexTypeAttribute = qSharedPointerCast<ComplexType>(pAttribute->getType());
				if(pAttribute->isList()){
					szIterator = "iter_" + QString::number(iIteratorCount++);
					szDefinition += "\tif(" + pAttribute->getName() + "List" + ".size() > 0) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\"" CRLF;
					szDefinition += QString("\t\tQList<%0>::const_iterator %1;" CRLF).arg( pComplexTypeAttribute->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\t\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pAttribute->getName());
					szDefinition += QString("\t\t\tif(!(*%0)->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t\tszValue += (*%0)->serialize() + \" \";" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t}" CRLF);
					szDefinition += "\t\t}" CRLF;
					szDefinition += "\t\tszValue +=\"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}else{
					QString szVariableName = "_" + ModelUtils::getUncapitalizedName(pAttribute->getName());
					szDefinition += "\tif(!" + szVariableName + ".isNull()) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\" + " + szVariableName + ".serialize() + \"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}
			}
		}
	}

	// Process extension type
	const auto& pExtensionType = pComplexType->getExtensionType();
	if(!pExtensionType.isNull()) {
		QString szExtensionName;
		if(pExtensionType->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pExtensionType);
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = pExtensionType->getNameWithNamespace();
		}

		if(pComplexType->isExtensionTypeList()){
			szIterator = "iter_" + QString::number(iIteratorCount++);
			szDefinition += "\tQList<" + szExtensionName + ">::const_iterator " + szIterator + " = constBegin();" CRLF;
			szDefinition += "\twhile(" + szIterator + " != constEnd()){" CRLF;
			szDefinition += "\t\tszValue += (*" + szIterator + ").serialize(" +
					QString(((pExtensionType->getTypeMode() == Type::TypeSimple) ? "" : "true")) +
					") + \" \";" CRLF;
			szDefinition += "\t\t++" + szIterator + ";" CRLF;
			szDefinition += "\t}" CRLF;
		}else{
			szDefinition += "\tszValue += " +
					szExtensionName +
					"::serialize(" +
					((pExtensionType->getTypeMode() == Type::TypeSimple) ? "" : "true") +
					");" CRLF;
		}
	}else{
		szDefinition += "\t\tszValue += \">\";" CRLF;
	}

	// Process list element
	const auto& pListElement = pComplexType->getElementList();
	for(elem = pListElement->constBegin(); elem != pListElement->constEnd(); ++elem)
	{
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szIterator = "iter_" + QString::number(iIteratorCount++);
				if(pSimpleType->isEnumeration()){
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getNameWithNamespace()).arg(szIterator);
				}else{
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getCPPTypeNameString()).arg(szIterator);
				}
				szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
				szDefinition += QString("\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
				szDefinition += QString("\t\t\tszValue += \"<" + szTargetNamespace + ":" + pElement->getName() + ">\" + %0->serialize() + \"</" + szTargetNamespace + ":" + pElement->getName() + ">\";" CRLF).arg(szIterator);
				szDefinition += QString("\t\t}" CRLF);
				szDefinition += "\t}" CRLF;
			} else{
				szDefinition += "\tif(!" + pElement->getVariableName() + ".isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + szTargetNamespace + ":" + pElement->getName() + ">\" + " + pElement->getVariableName() + ".serialize() + \"</" + szTargetNamespace + ":" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexTypeElement = qSharedPointerCast<ComplexType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szIterator = "iter_" + QString::number(iIteratorCount++);
				if(pElement->isPointer()){
					szDefinition += QString("\tQList<%0*>::const_iterator %1;" CRLF).arg( pComplexTypeElement->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
					szDefinition += QString("\t\tif(*%0 && !(*%0)->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += (*%0)->serialize();" CRLF).arg(szIterator);
					szDefinition += QString("\t\t}" CRLF);
					szDefinition += "\t}" CRLF;
				}else{
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg( pComplexTypeElement->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
					szDefinition += QString("\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += \"<%0:%1\";" CRLF).arg(szTargetNamespace).arg(pElement->getName());
					szDefinition += QString("\t\t\tszValue += %0->serialize(true);" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += \"</%0:%1>\";" CRLF).arg(szTargetNamespace).arg(pElement->getName());
					szDefinition += QString("\t\t}" CRLF);
					szDefinition += "\t}" CRLF;
				}
			} else{
				szDefinition += "\tif(" + ((pElement->isNested() || pElement->isPointer()) ? pElement->getVariableName() + " && " : "") +
						"!" + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + szTargetNamespace + ":" + pElement->getName() + "\" + " + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "serialize(true) + \"</" + szTargetNamespace + ":" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}
	szDefinition += "\tif(!bOnlyContent) {" CRLF;
	szDefinition += "\t\tszValue += \"</%1:%2>\";" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += "\treturn szValue;" CRLF;
	szDefinition += "}" CRLF;
	os << szDefinition.arg(szClassName).arg(szTargetNamespace).arg(pComplexType->getLocalName()) << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionDeserializer(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName, const QString& szTargetNamespace) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition;

	szDefinition = ""
	"void %0::deserialize(QDomElement& element)" CRLF
	"{" CRLF;

	const auto& pExtensionType = pComplexType->getExtensionType();
	if(!pExtensionType.isNull()) {
		QString szExtensionName;
		if(pExtensionType->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pExtensionType);
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = pExtensionType->getNameWithNamespace();
		}

		if(pComplexType->isExtensionTypeList()){
			szDefinition += "\tQString szElement = element.text();" CRLF;
			szDefinition += "\tQStringList listElements = szElement.split(\" \");" CRLF;
			szDefinition += "\tfor(int i = 0; i < listElements.size(); ++i){" CRLF;
			szDefinition += "\t\tQDomAttr attr;" CRLF;
			szDefinition += "\t\tattr.setValue(listElements[i]);" CRLF;
			szDefinition += "\t\t" + szExtensionName + " item;" CRLF;
			szDefinition += "\t\titem.deserialize(attr);" CRLF;
			szDefinition += "\t\tappend(item);" CRLF;
			szDefinition += "\t}" CRLF;
		}else{
			szDefinition += "\t" + szExtensionName + "::deserialize(element);" CRLF;
		}
	}

	//Not found other cases so we supposed attribute is only a simpleType
	const auto& pListAttribute = pComplexType->getAttributeList();
	for(attr = pListAttribute->constBegin(); attr != pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if( pAttribute->getType() && pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
			if(pAttribute->isList()){
				szDefinition += "\tif(!element.attributeNode(\"" + pAttribute->getName() + "\").isNull()) {" CRLF;
				szDefinition += "\t\tQDomAttr attribute = element.attributeNode(\"" + pAttribute->getName() + "\");" CRLF;
				szDefinition += "\t\tQString szAttrValue = attribute.value();" CRLF;
				szDefinition += "\t\tif(!szAttrValue.isEmpty()){" CRLF;
				szDefinition += "\t\t\tQStringList szValues = szAttrValue.split(\" \");" CRLF;
				szDefinition += "\t\t\tfor(int i = 0; i < szValues.size(); ++i){" CRLF;
				szDefinition += "\t\t\t\tif(!szValues[i].isEmpty()){" CRLF;
				szDefinition += "\t\t\t\t\t" + pSimpleType->getCPPTypeNameString() + " tmp;" CRLF;
				szDefinition += "\t\t\t\t\ttmp.setValue(szValues[i]);" CRLF;
				szDefinition += "\t\t\t\t\tadd" + ModelUtils::getCapitalizedName(pAttribute->getName()) + "(tmp);" CRLF;
				szDefinition += "\t\t\t\t}" CRLF;
				szDefinition += "\t\t\t}" CRLF;
				szDefinition += "\t\t}" CRLF;
				szDefinition += "\t}" CRLF;
			}else{
				szDefinition += "\tif(!element.attributeNode(\"" + pAttribute->getName() + "\").isNull()) {" CRLF;
				szDefinition += "\t\t" + pSimpleType->getVariableName() + ".deserialize(element.attributeNode(\"" + pAttribute->getName() + "\"));" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}
	szDefinition += CRLF;
	szDefinition += "\tQDomElement child = element.firstChild().toElement();" CRLF;
	szDefinition += "\twhile(!child.isNull()) {" CRLF;

	const auto& pListElement = pComplexType->getElementList();
	for(elem = pListElement->constBegin(); elem != pListElement->constEnd(); ++elem) {
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szDefinition += "\t\tif(child.tagName() == \"" + pComplexType->getNamespace() + ":" + pElement->getName() + "\") {" CRLF;
				if(pSimpleType->isEnumeration()){
					szDefinition += "\t\t\t" + pSimpleType->getNameWithNamespace() + " item;" CRLF;
				}else{
					szDefinition += "\t\t\t" + pSimpleType->getCPPTypeNameString() + " item;" CRLF;
				}
				szDefinition += "\t\t\titem.deserialize(child);" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
				szDefinition += "\t\t}" CRLF;

			} else{
				szDefinition += "\t\tif((child.tagName() == \"" + pComplexType->getNamespace() + ":" + pElement->getName() + "\")" +
						" || (child.tagName().endsWith(\":" + pElement->getName() + "\"" + "))"
						" || (child.tagName() == \"" + pElement->getName() + "\"" + ")){" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + ".deserialize(child);" CRLF;
				szDefinition += "\t\t}" CRLF;
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexTypeElement = qSharedPointerCast<ComplexType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				if(pElement->isPointer()){
					szDefinition += "\t\tif(child.tagName() == \"" + pComplexTypeElement->getNamespace() + ":" + pElement->getName() + "\") {" CRLF;
					szDefinition += "\t\t\t" + pComplexTypeElement->getNameWithNamespace() + "* item = new " + pComplexTypeElement->getNameWithNamespace() + "();" CRLF;
					szDefinition += "\t\t\titem->deserialize(child);" CRLF;
					szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
					szDefinition += "\t\t}" CRLF;
				}else{
					szDefinition += "\t\tif((child.tagName() == \"" + pComplexTypeElement->getNamespace() + ":" + pElement->getName() + "\") " +
							" || child.tagName().endsWith(\":" + pElement->getName() + "\"" + ")){" CRLF;
					szDefinition += "\t\t\t" + pComplexTypeElement->getNameWithNamespace() + " item;" CRLF;
					szDefinition += "\t\t\titem.deserialize(child);" CRLF;
					szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
					szDefinition += "\t\t}" CRLF;
				}
			} else{
				szDefinition += "\t\tif((child.tagName() == \"" + pComplexTypeElement->getNamespace() + ":" + pElement->getName() + "\") " +
						" || child.tagName().endsWith(\":" + pElement->getName() + "\"" + ")){" CRLF;
				if((pElement->isNested() || pElement->isPointer())){
					szDefinition += "\t\t\tif(!" + pElement->getVariableName() + "){" CRLF;
					szDefinition += "\t\t\t\t" + pElement->getVariableName() + " = new " + pElement->getType()->getLocalName() + "();" CRLF;
					szDefinition += "\t\t\t}" CRLF;
				}
				szDefinition += "\t\t\t" + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "deserialize(child);" CRLF;
				szDefinition += "\t\t}" CRLF;
			}
		}
	}
	szDefinition += "\t\tchild = child.nextSibling().toElement();" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += "}" CRLF;

	os << szDefinition.arg(szClassName) << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationIsNull(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	os << "\tbool isNull() const;" << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionIsNull(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition = ""
	"bool %0::isNull() const" CRLF
	"{" CRLF
	"\treturn ";
	QString szCond = "";

	const auto& pExtensionType = pComplexType->getExtensionType();
	if(!pExtensionType.isNull()) {
		QString szExtensionName;
		if(pExtensionType->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pExtensionType);
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = pExtensionType->getNameWithNamespace();
		}

		if(pComplexType->isExtensionTypeList()){
			szCond += "isEmpty() && ";
		}else{
			szCond += szExtensionName + "::isNull() && ";
		}
	}

	const auto& pListAttribute = pComplexType->getAttributeList();
	for(attr = pListAttribute->constBegin(); attr != pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if(pAttribute->getType()){
			if( pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
				if(pAttribute->isList()){
					szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
				}else{
					szCond += pSimpleType->getVariableName() + ".isNull() && ";
				}

			}else if( pAttribute->getType()->getTypeMode() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());
				if(pAttribute->isList()){
					szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
				}else{
					QString szVariableName = "_" + ModelUtils::getUncapitalizedName(pAttribute->getName());
					szCond += szVariableName + ".isNull() && ";
				}
			}
		}
	}

	const auto& pListElement = pComplexType->getElementList();
	for(elem = pListElement->constBegin(); elem != pListElement->constEnd(); ++elem) {
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if( !pElement->getType()) {
			continue;
		}
		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());
			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szCond += pElement->getVariableName() + "List.isEmpty() && ";
			}else{
				szCond += pElement->getVariableName() + ".isNull() && ";
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szCond += pElement->getVariableName() + "List.isEmpty() && ";
			}else{
				if((pElement->isNested() || pElement->isPointer())){
					szCond += "(!" + pElement->getVariableName() + " || (" + pElement->getVariableName() + " && " + pElement->getVariableName() + "->isNull())) && ";
				}else{
					szCond += pElement->getVariableName() + ".isNull() && ";
				}
			}
		}
	}
	if(szCond.length() > 1) {
		szDefinition += szCond.left(szCond.length() - 4);
		szDefinition += ";" CRLF;
	}else{
		szDefinition += "true;" CRLF;
	}
	szDefinition += "}" CRLF;

	os << szDefinition.arg(szClassName) << CRLF;
}

void QtCppTargetEngine::doWriteDeclarationGetNamespaceDeclaration(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	os << "\tstatic QList<QString> getNamespaceDeclaration();" << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionGetNamespaceDeclaration(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName) const
{
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition = ""
	"QList<QString> %0::getNamespaceDeclaration()" CRLF
	"{" CRLF;
	szDefinition += "\tQList<QString> listNamespaceDeclaration;" CRLF;

	const auto& pExtentionType = pComplexType->getExtensionType();
	if(!pExtentionType.isNull()){
		QString szExtensionName;
		if(pExtentionType->getTypeMode() == Type::TypeComplex){
			szDefinition += "\tlistNamespaceDeclaration.append(" + pExtentionType->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;
		}
	}

	const auto& pListElement = pComplexType->getElementList();
	for(elem = pListElement->constBegin(); elem != pListElement->constEnd(); ++elem){
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if((pElement->getType()->getTypeMode() == Type::TypeComplex) &&
				(pComplexType->getLocalName(true) != pElement->getType()->getLocalName(true))){
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
			szDefinition += "\tlistNamespaceDeclaration.append(" + pComplexType->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;
				}
	}

	szDefinition += "\tlistNamespaceDeclaration.append(\"xmlns:" + pComplexType->getNamespace() + "=\\\"" + pComplexType->getNamespaceUri() + "\\\"\");" CRLF;
	szDefinition += "\treturn listNamespaceDeclaration;" CRLF;
	szDefinition += "}" CRLF;

	os << szDefinition.arg(szClassName) << CRLF;
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

void QtCppTargetEngine::doWriteDeclarationGetterSetter(QTextStream& os, const ElementSharedPtr& pElement) const
{
	QString szElementName = pElement->getName();
	QString szFuncName = ModelUtils::getCapitalizedName(szElementName);
	QString szParamType;
	QString szParamName = ModelUtils::getUncapitalizedName(szElementName);

	const auto& pType = pElement->getType();
	const int iMaxOccurs = pElement->getMaxOccurs();
	const bool bIsNested = pElement->isNested();
	const bool bIsPointer = pElement->isPointer();

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		szParamType = pSimpleType->getCPPTypeNameString();
		szParamName = ModelUtils::getUncapitalizedName(szElementName);
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			doWriteDeclarationSetterList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationAddList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetterList(os, szFuncName, szParamType, GetterReturnModeConst);
		}else{
			doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			if (pSimpleType->isEnumeration()) {
				doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeDefault);
			}else{
				doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
			}
		}
	}else {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			szParamType = pComplexType->getNameWithNamespace();
			if(bIsPointer){
				doWriteDeclarationSetterList(os, szFuncName, szParamType, szParamName, SetterParamModePointer);
				doWriteDeclarationAddList(os, szFuncName, szParamType, szParamName, SetterParamModePointer);
				doWriteDeclarationGetterList(os, szFuncName, szParamType, GetterReturnModePointer);
			}else{
				doWriteDeclarationSetterList(os, szFuncName, szParamType, szParamName, SetterParamModeConst2);
				doWriteDeclarationAddList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
				doWriteDeclarationGetterList(os, szFuncName, szParamType, GetterReturnModeConst);
			}
		}else{
			if(bIsNested || bIsPointer){
				szParamType = pComplexType->getLocalName();
				doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModePointer);
				doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModePointer);
			}else{
				szParamType = pComplexType->getNameWithNamespace();
				doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
				doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
			}
		}
	}
}

void QtCppTargetEngine::doWriteDefinitionGetterSetter(QTextStream& os, const ElementSharedPtr& pElement, const QString& szClassName) const
{
	QString szElementName = pElement->getName();
	QString szFuncName = ModelUtils::getCapitalizedName(szElementName);
	QString szParamType;
	QString szParamName = ModelUtils::getUncapitalizedName(szElementName);
	QString szMemberType;
	QString szMemberName;

	const auto& pType = pElement->getType();
	const int iMaxOccurs = pElement->getMaxOccurs();
	const bool bIsNested = pElement->isNested();
	const bool bIsPointer = pElement->isPointer();

	if(pType->getTypeMode() == Type::TypeSimple)
	{
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		szParamType = pSimpleType->getCPPTypeNameString();
		szMemberType = pSimpleType->getCPPTypeNameString();
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			szMemberName = pElement->getVariableNameList();
			doWriteDefinitionSetterList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionAddList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionGetterList(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
		}else{
			szMemberName = pElement->getVariableName();
			doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			if (pSimpleType->isEnumeration()) {
				doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeDefault);
			}else {
				doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
			}
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			szParamType = pComplexType->getNameWithNamespace();
			szMemberName = pElement->getVariableNameList();
			szMemberType = pComplexType->getNameWithNamespace();
			if(bIsPointer){
				doWriteDefinitionSetterList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModePointer);
				doWriteDefinitionAddList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModePointer);
				doWriteDefinitionGetterList(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModePointer);
			}else{;
				doWriteDefinitionSetterList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst2);
				doWriteDefinitionAddList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
				doWriteDefinitionGetterList(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
			}
		}else{
			szMemberName = pElement->getVariableName();
			if(bIsNested || bIsPointer){
				szParamType = pComplexType->getLocalName();
				szMemberType = pComplexType->getLocalName();
				doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModePointer);
				doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModePointer);
			}else{
				szParamType = pComplexType->getNameWithNamespace();
				szMemberType = pComplexType->getNameWithNamespace();
				doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
				doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
			}
		}
	}
}

void QtCppTargetEngine::doWriteDeclarationVariable(QTextStream& os, const ElementSharedPtr& pElement) const
{
	QString szDeclaration;

	QString szVariableName = pElement->getVariableName();
	const auto& pType = pElement->getType();
	const int iMaxOccurs = pElement->getMaxOccurs();
	const bool bIsNested = pElement->isNested();
	const bool bIsPointer = pElement->isPointer();

	QtCppWriter langWriter(os);

	if(pType->getTypeMode() == Type::TypeSimple)
	{
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			langWriter.writeDeclarationVariableList(pSimpleType->getCPPTypeNameString(), szVariableName+"List");
		} else {
			langWriter.writeDeclarationVariable(pSimpleType->getCPPTypeNameString(), szVariableName);
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(iMaxOccurs > 1 || iMaxOccurs == -1) {
			langWriter.writeDeclarationVariableList(pType->getNameWithNamespace(), szVariableName+"List", bIsPointer);
		} else {
			const auto& szTypeName =  (bIsNested || bIsPointer) ? pType->getLocalName() : pType->getNameWithNamespace();
			langWriter.writeDeclarationVariable(szTypeName, szVariableName, (bIsNested || bIsPointer));
		}
		szDeclaration += ";";
	}
}

void QtCppTargetEngine::doWriteDeclarationGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute) const
{
	QString szDeclaration;

	const auto& szAttributeName = pAttribute->getName();
	QString szFuncName = ModelUtils::getCapitalizedName(szAttributeName);
	QString szParamType;
	QString szParamName = ModelUtils::getUncapitalizedName(szAttributeName);

	const auto& pType = pAttribute->getType();
	const auto& bIsList = pAttribute->isList();

	if(pType->getTypeMode() == Type::TypeSimple) {
		//os << "\t//simple" << CRLF;
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(bIsList){
			szParamType = pSimpleType->getCPPTypeNameString();
			doWriteDeclarationSetterList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationAddList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetterList(os, szFuncName, szParamType, GetterReturnModeConst);
		}else if(pSimpleType->isEnumeration()) {
			//os << "\t//simple enum" << CRLF;
			QString szLocalName = pSimpleType->getLocalName();
			szFuncName = ModelUtils::getCapitalizedName(szLocalName);
			szParamType = pSimpleType->getCPPTypeNameString();
			szParamName = ModelUtils::getUncapitalizedName(szLocalName);
			doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
		} else {
			//os << "\t//simple test" << CRLF;
			QString szLocalName = pSimpleType->getLocalName();
			szFuncName = ModelUtils::getCapitalizedName(szLocalName);
			szParamType = pSimpleType->getCPPTypeNameValuesString();
			szParamName = ModelUtils::getUncapitalizedName(szLocalName);
			doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
		}
	}else if(pType->getTypeMode() == Type::TypeComplex) {
		//os << "\t//complex" << CRLF;
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(bIsList){
			szParamType = pComplexType->getNameWithNamespace();
			doWriteDeclarationSetterList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationAddList(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetterList(os, szFuncName, szParamType, GetterReturnModeConst);
		}else{
			QString szLocalName = (szAttributeName.isEmpty() ? pComplexType->getLocalName() : szAttributeName);
			szFuncName = ModelUtils::getCapitalizedName(szLocalName);
			szParamType = pComplexType->getNameWithNamespace();
			szParamName = ModelUtils::getUncapitalizedName(szLocalName);
			doWriteDeclarationSetter(os, szFuncName, szParamType, szParamName, SetterParamModeConst);
			doWriteDeclarationGetter(os, szFuncName, szParamType, GetterReturnModeConst);
		}
	}
}

void QtCppTargetEngine::doWriteDefinitionGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute, const QString& szClassName) const
{
	QString szDefinition;

	const auto& szAttributeName = pAttribute->getName();
	QString szFuncName = ModelUtils::getCapitalizedName(szAttributeName);
	QString szParamType;
	QString szParamName = ModelUtils::getUncapitalizedName(szAttributeName);
	QString szMemberType;
	QString szMemberName = pAttribute->getVariableNameList();

	const auto& pType = pAttribute->getType();
	const auto& bIsList = pAttribute->isList();

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		szParamType = pSimpleType->getCPPTypeNameString();
		szMemberType = pSimpleType->getCPPTypeNameString();

		if(bIsList){
			doWriteDefinitionSetterList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionAddList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionGetterList(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
		}else if(!pSimpleType->isEnumeration()) {
			doWriteDefinitionGetterSetter(os, pSimpleType, szClassName);
		} else {
			QString szLocalName = pSimpleType->getLocalName();
			szFuncName = ModelUtils::getCapitalizedName(szLocalName);
			szParamType = pSimpleType->getCPPTypeNameString();
			szParamName = ModelUtils::getUncapitalizedName(szLocalName);
			szMemberName = pSimpleType->getVariableName();
			doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
		}
	}else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		szParamType = pComplexType->getNameWithNamespace();
		if(bIsList){
			szMemberType = pComplexType->getNameWithNamespace();
			doWriteDefinitionSetterList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionAddList(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionGetterList(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
		}else{
			QString szLocalName = (szAttributeName.isEmpty() ? pComplexType->getLocalName() : szAttributeName);
			szFuncName = ModelUtils::getCapitalizedName(szLocalName);
			szMemberType = pComplexType->getNameWithNamespace();
			szMemberName = "_" + ModelUtils::getUncapitalizedName(szLocalName);
			doWriteDefinitionSetter(os, szClassName, szFuncName, szParamType, szParamName, szMemberName, SetterParamModeConst);
			doWriteDefinitionGetter(os, szClassName, szFuncName, szMemberType, szMemberName, GetterReturnModeConst);
		}
	}
}

void QtCppTargetEngine::doWriteDeclarationVariable(QTextStream& os, const AttributeSharedPtr& pAttribute) const
{
	QString szDeclaration;

	const auto& szAttributeName = pAttribute->getName();

	const auto& pType = pAttribute->getType();
	const auto& bIsList = pAttribute->isList();

	QtCppWriter langWriter(os);

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(bIsList){
			langWriter.writeDeclarationVariableList(pSimpleType->getCPPTypeNameString(), pAttribute->getVariableName()+"List");
		}else{
			langWriter.writeDeclarationVariable(pSimpleType->getCPPTypeNameString(), pSimpleType->getVariableName());
		}
	}else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		if(bIsList){
			langWriter.writeDeclarationVariableList(pComplexType->getNameWithNamespace(), pAttribute->getVariableName()+"List");
		}else{
			QString szVarName;
			if(!szAttributeName.isEmpty()){
				szVarName = "_" + ModelUtils::getUncapitalizedName(szAttributeName);
			}else{
				szVarName = pComplexType->getVariableName();
			}
			langWriter.writeDeclarationVariable(pComplexType->getNameWithNamespace(), szVarName);
		}
	}
}

void QtCppTargetEngine::doWriteDeclarationOperation(QTextStream& os, const OperationSharedPtr& pOperation) const
{
	QString szDeclaration;

	const auto& szName = pOperation->getName();
	const auto& pInputMessage = pOperation->getInputMessage();
	const auto& pOutputMessage = pOperation->getOutputMessage();

	const auto& pSoapEnvFaultType = pOperation->getSoapEnvelopeFaultType();

	if(pInputMessage->getParameter() && pOutputMessage->getParameter())
	{
		szDeclaration += "bool ";
		szDeclaration += szName;
		szDeclaration += "(const ";
		szDeclaration += pInputMessage->getParameter()->getNameWithNamespace();
		szDeclaration += "& ";
		szDeclaration += pInputMessage->getParameter()->getLocalName();
		szDeclaration += ", ";
		szDeclaration += pOutputMessage->getParameter()->getNameWithNamespace();
		szDeclaration += "& ";
		szDeclaration += pOutputMessage->getParameter()->getLocalName();
		if(pSoapEnvFaultType){
			szDeclaration += ", ";
			szDeclaration += pSoapEnvFaultType->getNameWithNamespace();
			szDeclaration += "& ";
			szDeclaration += pSoapEnvFaultType->getLocalName();
		}
		szDeclaration += ");";
	}

	os << "\t" << szDeclaration << CRLF;
}

void QtCppTargetEngine::doWriteDefinitionOperation(QTextStream& os, const OperationSharedPtr& pOperation, const QString& szClassname, const QString& szNamespace) const
{
	const auto& szName = pOperation->getName();
	const auto& pInputMessage = pOperation->getInputMessage();
	const auto& pOutputMessage = pOperation->getOutputMessage();

	if(!pInputMessage->getParameter() || !pOutputMessage->getParameter()){
		return;
	}

	const auto& pSoapEnvFaultType = pOperation->getSoapEnvelopeFaultType();
	const auto& szSoapAction = pOperation->getSoapAction();

	QString szInputName = ModelUtils::getUncapitalizedName(pInputMessage->getParameter()->getLocalName());
	QString szOutputName = ModelUtils::getUncapitalizedName(pOutputMessage->getParameter()->getLocalName());

	QString szDefinition;
	szDefinition += "bool " + szClassname + "::" + szName + "(const " + pInputMessage->getParameter()->getNameWithNamespace() +
			"& " + szInputName + ", " + pOutputMessage->getParameter()->getNameWithNamespace() +
			"& " + szOutputName;

	if(pSoapEnvFaultType){
		szDefinition += ", " + pSoapEnvFaultType->getNameWithNamespace() + "& " + pSoapEnvFaultType->getLocalName() + ")" CRLF;
	}else{
		szDefinition += ")" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\tbool bGoOn = true;" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tQNetworkRequest request = buildNetworkRequest();" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"Content-Type\").toLatin1(), QString(\"application/soap+xml; charset=utf-8; action=\\\"" + szSoapAction + "\\\"\").toLatin1());" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"Accept-Encoding\").toLatin1(), QString(\"gzip, deflate\").toLatin1());" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"SoapAction\").toLatin1(), QString(\"" + szSoapAction + "\").toLatin1());" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tQByteArray soapMessage = buildSoapMessage(" + szInputName + ".serialize(), " + pInputMessage->getParameter()->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;

	// Debug request
	szDefinition += "\tif(m_bDebug){" CRLF;
	szDefinition += "\t\tqWarning(\"Message:\\n%s\", qPrintable(QString::fromUtf8(soapMessage)));" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += CRLF;
	szDefinition += "\tIQueryExecutorResponse response = m_pQueryExecutor->execQuery(request, soapMessage);" CRLF;
	szDefinition += "\tQString szErrorMsg;" CRLF;
	szDefinition += "\tint iErrorLine = -1;" CRLF;
	szDefinition += "\tint iErrorColumn = -1;" CRLF;
	szDefinition += "\tQDomDocument doc;" CRLF;
	szDefinition += "\tQMap<QString, QString> namespaceRoutingMap;" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tif(doc.setContent(response.getResponse(), &szErrorMsg, &iErrorLine, &iErrorColumn)){" CRLF;
	szDefinition += "\t\tnamespaceRoutingMap = buildNamespaceRoutingMap(doc);" CRLF;
	szDefinition += CRLF;
	if(pSoapEnvFaultType){
		szDefinition += "\t\tQString szNamespace = namespaceRoutingMap.value(SOAP_ENV_URI, " + szNamespace + "::" + pSoapEnvFaultType->getNameWithNamespace() + "_TargetNamespace);" CRLF;
		szDefinition += "\t\tQString szFaultTagName = szNamespace + \":Fault\";" CRLF;
		szDefinition += "\t\tif(doc.elementsByTagName(szFaultTagName).size() > 0){" CRLF;
		szDefinition += "\t\t\tQDomElement root = doc.elementsByTagName(szFaultTagName).at(0).toElement();" CRLF;
		szDefinition += "\t\t\tFault.deserialize(root);" CRLF;
		szDefinition += "\t\t\tbGoOn = false;" CRLF;
		szDefinition += "\t\t}else{" CRLF;
	}
	szDefinition += "\t\t" + QString(pSoapEnvFaultType ? "\t" : "") + "QDomElement root = doc.elementsByTagName(namespaceRoutingMap.value(SOAP_ENV_URI, DEFAULT_SOAP_ENV_NAMESPACE) + \":Body\").at(0).firstChildElement();" CRLF;
	szDefinition += "\t\t" + QString(pSoapEnvFaultType ? "\t" : "") + szOutputName + ".deserialize(root);" CRLF;
	if(pSoapEnvFaultType){
		szDefinition += "\t\t}" CRLF;
	}
	szDefinition += "\t}else{" CRLF;
	szDefinition += "\t\tbGoOn = false;" CRLF;
	szDefinition += "\t\tqWarning(\"[" + szNamespace + "::" + szName + "] Error during parsing response : %s (%d:%d)\", qPrintable(szErrorMsg), iErrorLine, iErrorColumn);" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tif(response.getHttpStatusCode() != 200){" CRLF;
	szDefinition += "\t\tbGoOn = false;" CRLF;
	szDefinition += "\t\tqWarning(\"[" + szNamespace + "::" + szName + "] Error with HTTP status code: %d\", response.getHttpStatusCode());" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += "\tif(m_bDebug){" CRLF;
	szDefinition += "\t\tqWarning(\"Response:\\n%s\", qPrintable(QString::fromUtf8(response.getResponse())));" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += "\treturn bGoOn;" CRLF;
	szDefinition += "}" CRLF;

	os << szDefinition << CRLF;
}

void QtCppTargetEngine::startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const
{
	QString szExtendedClassName = getBaseClassName(m_szPrefix, pComplexType);

	// Start class
	doWriteDeclarationClassStart(os, szClassName, szExtendedClassName);
	// Constructor/destructor declaration
	doWriteDeclarationClassInitializers(os, szClassName, false);
}

void QtCppTargetEngine::endCppClass(QTextStream& os, const QString& szClassName) const
{
	doWriteDeclarationClassEnd(os, szClassName);
}