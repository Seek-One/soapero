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

	os << "class " << szClassname << " : public Service" CRLF;
	os << "{" << CRLF;
	os << "public:" << CRLF;
	os << "\t" << szClassname << "();" << CRLF;
	os << "\tvirtual ~" << szClassname << "();" << CRLF;
	os << CRLF;

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

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
	}

	os << "namespace MSG {" << CRLF;
	os << CRLF;

	ComplexTypeSharedPtr pComplexType;
	if(pType->getTypeMode() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}
	if(!pComplexType.isNull())
	{
		// Write namespace
		os << "const QString " << szClassName << "TargetNamespace = \"" << pElement->getNamespace() << "\";" CRLF;
		os << "const QString " << szClassName << "TargetNamespaceUri = \"" << pElement->getNamespaceUri() << "\";" CRLF;
		os << CRLF;

		startCppClass(os, szClassName, pComplexType);
		doWriteHeaderClassContent(os, pComplexType);
		endCppClass(os);
	}

	os << "} // MSG" << CRLF;

	if(!szNamespace.isEmpty()){
		os << "} // " << szNamespace << CRLF;
	}
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

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
	}

	os << "namespace MSG {" << CRLF;
	os << CRLF;

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		doWriteCppClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	os << "} // MSG" << CRLF;

	if(!szNamespace.isEmpty()){
		os << "} // " << szNamespace << CRLF;
	}
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
					qWarning("[TypeListBuilder] Union type not found: %s:%s", qPrintable(pUnionTypeRef->getNamespace()), qPrintable(pUnionTypeRef->getTypeName()));
				}
			}
		}
		if(pSimpleType->hasVariableType()) {
			os << "#include \"" << getHeaderFilePath(pSimpleType, FileCategory_Type) << "\"" << CRLF;
		}
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
	}
}

void QtCppTargetEngine::doWriteHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
	}

	os << "namespace TYPES {" << CRLF;
	os << CRLF;

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		os << "const QString " << pSimpleType->getLocalName(true) << "TargetNamespace = \"" << pSimpleType->getNamespace() << "\";" CRLF;
		os << "const QString " << pSimpleType->getLocalName(true) << "TargetNamespaceUri = \"" << pSimpleType->getNamespaceUri() << "\";" CRLF;
		os << CRLF;

		os << "class " << szClassname << CRLF;
		os << "{" << CRLF;
		os << "public:" << CRLF;

		if(pSimpleType->isEnumeration()) {
			os << "\t" << pSimpleType->getEnumerationDeclaration() << CRLF;
			os << CRLF;
		}

		os << "\t" << szClassname << "();" << CRLF;
		os << "\tvirtual ~" << szClassname << "();" << CRLF;
		os << CRLF;

		doWriteHeaderClassContent(os, pSimpleType);

		os << "};" << CRLF;
		os << CRLF;

	} else if(pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		os << "const QString " << pComplexType->getLocalName(true) << "TargetNamespace = \"" << pComplexType->getNamespace() << "\";" CRLF;
		os << "const QString " << pComplexType->getLocalName(true) << "TargetNamespaceUri = \"" << pComplexType->getNamespaceUri() << "\";" CRLF;
		os << CRLF;

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
		endCppClass(os);
	}

	os << "} // TYPES " << CRLF;

	if(!szNamespace.isEmpty()){
		os << "} // " << szNamespace << CRLF;
	}
}

void QtCppTargetEngine::doWriteCppClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
	}

	os << "namespace TYPES {" << CRLF;
	os << CRLF;

	if(pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

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

		doWriteCppClass(os, pSimpleType);

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
			QString szExtensionName;
			if(pComplexType->isExtensionTypeList()){
				szExtensionName = "QList<";
			}
			if(pComplexType->getExtensionType()->getTypeMode() == Type::TypeSimple){
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
				szExtensionName += pSimpleType->getCPPTypeNameString();
			}else{
				szExtensionName += pComplexType->getExtensionType()->getNameWithNamespace();
			}
			if(pComplexType->isExtensionTypeList()){
				szExtensionName += ">";
			}
			QString szExtendedClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;

			os << szClassname << "::" << szClassname << "() : " << szExtendedClassname << "() {}" << CRLF;
			os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		}
		os << CRLF;

		doWriteCppClassContent(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	os << "} // TYPES" << CRLF;

	if(!szNamespace.isEmpty()){
		os << "} // " << szNamespace << CRLF;
	}
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
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
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
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
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
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
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
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
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

void QtCppTargetEngine::startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const
{
	os << "class " << szClassName;
	if(!pComplexType->getExtensionType().isNull())
	{
		QString szExtendedClassname;
		if(pComplexType->isExtensionTypeList()){
			szExtendedClassname = "QList<";
		}
		if(pComplexType->getExtensionType()->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
			szExtendedClassname += pSimpleType->getCPPTypeNameString();
		}else{
			QString szExtensionName = pComplexType->getExtensionType()->getNameWithNamespace();
			szExtendedClassname += (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;
		}
		if(pComplexType->isExtensionTypeList()){
			szExtendedClassname += ">";
		}

		os << " : public " << szExtendedClassname;
	}
	os << CRLF;
	os << "{" << CRLF;
	os << "public:" << CRLF;
	os << "\t" << szClassName << "();" << CRLF;
	os << "\tvirtual ~" << szClassName << "();" << CRLF;
	os << CRLF;
}

void QtCppTargetEngine::endCppClass(QTextStream& os) const
{
	os << "};" << CRLF;
	os << CRLF;
}