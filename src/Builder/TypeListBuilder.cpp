/*
 * TypeListBuilder.cpp
 *
 *  Created on: 17 juil. 2017
 *      Author: lgruber
 */

#include <QDateTime>
#include <QCoreApplication>
#include <QStringList>
#include <QDir>

#include "../Utils/StringUtils.h"
#include "TypeListBuilder.h"
#include "FileHelper.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

TypeListBuilder::TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement, const QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	m_pListType = pListType;
	m_pListElement = pListElement;
	m_pService = pService;

	m_pListGeneratedFiles = pListGeneratedFiles;

	m_szPrefix = "";
}

TypeListBuilder::~TypeListBuilder()
{

}

void TypeListBuilder::setPrefix(const QString& szPrefix)
{
	m_szPrefix = szPrefix;
}

QString TypeListBuilder::getPrefix() const
{
	return m_szPrefix;
}

void TypeListBuilder::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

QString TypeListBuilder::getNamespace() const
{
	return m_szNamespace;
}

void TypeListBuilder::setFilename(const QString& szFilename)
{
	m_szFilename = szFilename;
}

QString TypeListBuilder::getFilename() const
{
	return m_szFilename;
}

void TypeListBuilder::setDirname(const QString& szDirname)
{
	m_szDirname = szDirname;
}

QString TypeListBuilder::getDirname() const
{
	return m_szDirname;
}

QString TypeListBuilder::getHeaderPath(const QString& szNamespace, const QString& szCategory, const QString& szFilename, FileCategory iOrigin)
{
	QString szRootPath;
	if(iOrigin == FileCategory_Type || iOrigin == FileCategory_Message)
	{
		szRootPath = "../../";
	}
	return FileHelper::buildPath(szRootPath, szNamespace, szCategory, szFilename);
}

QString TypeListBuilder::getTypeHeaderPath(const TypeSharedPtr& pType, FileCategory iOrigin)
{
	if(pType->getClassType() == Type::TypeSimple){
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		return getSimpleTypeHeaderPath(pSimpleType, iOrigin);
	}else if(pType->getClassType() == Type::TypeComplex){
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		return getComplexTypeHeaderPath(pComplexType, iOrigin);
	}else{
		qWarning("[TypeListBuilder] Unknown type: %s", qPrintable(pType->getLocalName()));
		QString szFileName = getHeaderFileName(pType);
		return getHeaderPath(pType->getNamespace(), "types", szFileName, iOrigin);
	}
}

QString TypeListBuilder::getSimpleTypeHeaderPath(const SimpleTypeSharedPtr& pSimpleType, FileCategory iOrigin)
{
	QString szFileName;
	QString szNamespace;
	if(pSimpleType->hasVariableType()){
		szNamespace = getSimpleTypeNamespace(pSimpleType);
		szFileName = getHeaderFileName(pSimpleType);
		return getHeaderPath(szNamespace, "types", szFileName, iOrigin);
	}
	return QString();
}

QString TypeListBuilder::getComplexTypeHeaderPath(const ComplexTypeSharedPtr& pComplexType, FileCategory iOrigin)
{
	QString szFileName = getHeaderFileName((TypeSharedPtr)pComplexType);
	return getHeaderPath( pComplexType->getNamespace(), "types", szFileName, iOrigin);
}

QString TypeListBuilder::getRequestResponseElementHeaderPath(const RequestResponseElementSharedPtr& pRequestResponseElement, FileCategory iOrigin)
{
	QString szFileName = getHeaderFileName(pRequestResponseElement);
	return getHeaderPath( pRequestResponseElement->getNamespace(), "messages", szFileName, iOrigin);
}

QString TypeListBuilder::getDefine(const QString& szPrefix, const TypeSharedPtr& pType)
{
	QString szDefine;

	if(!szPrefix.isEmpty()){
		szDefine += StringUtils::secureString(szPrefix).toUpper() + "_";
	}

#ifdef WITH_DIR_CREATION
	if(!pType->getNamespace().isEmpty()){
		szDefine += StringUtils::secureString(pType->getNamespace()).toUpper() + "_";
	}
	if(!pType->getLocalName().isEmpty()){
		szDefine += StringUtils::secureString(pType->getLocalName()).toUpper() + "_";
	}
#else
	szDefine += pType->getQualifiedName().toUpper() + "_";
#endif

	szDefine += "H_";

	return szDefine;
}

QString TypeListBuilder::getDefine(const QString& szPrefix, const RequestResponseElementSharedPtr& pElement)
{
	QString szDefine;

	if(!szPrefix.isEmpty()){
		szDefine += StringUtils::secureString(szPrefix).toUpper() + "_";
	}

#ifdef WITH_DIR_CREATION
	if(!pElement->getNamespace().isEmpty()){
		szDefine += StringUtils::secureString(pElement->getNamespace()).toUpper() + "_";
	}
	if(!pElement->getLocalName().isEmpty()){
		szDefine += StringUtils::secureString(pElement->getLocalName()).toUpper() + "_";
	}
#else
	szDefine += StringUtils::secureString(pElement->getQualifiedName()).toUpper() + "_";
#endif

	szDefine += "H_";

	return szDefine;
}

QString TypeListBuilder::getHeaderFileName(const TypeSharedPtr& pType)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pType->getLocalName();
#else
	szClassName = pType->getQualifiedName();
#endif
	return FileHelper::buildFileName(pType->getNamespace(), szClassName, "h");
}

QString TypeListBuilder::getCppFileName(const TypeSharedPtr& pType)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pType->getLocalName();
#else
	szClassName = pType->getQualifiedName();
#endif
	return FileHelper::buildFileName(pType->getNamespace(), szClassName, "cpp");
}

QString TypeListBuilder::getHeaderFileName(const SimpleTypeSharedPtr& pSimpleType)
{
	QString szClassName;
	QString szNamespace;
#ifdef WITH_DIR_CREATION
	szClassName = pSimpleType->getExportedTypename();
	szNamespace = getSimpleTypeNamespace(pSimpleType);
	if(szNamespace == "xs"){
		szNamespace = ""; // Don't use prefix for simple type
	}
#else
	szFileName = pSimpleType->getVariableTypeFilenameString();
#endif
	return FileHelper::buildFileName(szNamespace, szClassName, "h");
}

QString TypeListBuilder::getSimpleTypeNamespace(const SimpleTypeSharedPtr& pSimpleType)
{
	QString szNamespace;
#ifdef WITH_DIR_CREATION
	szNamespace = pSimpleType->getExportedNamespace();
	if(szNamespace == "xsd" || szNamespace == "s"){
		szNamespace = "xs";
	}
#endif
	return szNamespace;
}

QString TypeListBuilder::getHeaderFileName(const RequestResponseElementSharedPtr& pElement)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pElement->getLocalName();
#else
	szClassName = pElement->getQualifiedName();
#endif
	return FileHelper::buildFileName(pElement->getNamespace(), szClassName, "h");
}

QString TypeListBuilder::getCppFileName(const RequestResponseElementSharedPtr& pElement)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pElement->getLocalName();
#else
	szClassName = pElement->getQualifiedName();
#endif
	return FileHelper::buildFileName(pElement->getNamespace(), szClassName, "cpp");
}

void TypeListBuilder::buildHeaderFiles()
{
	TypeList::const_iterator type;
	RequestResponseElementList::const_iterator iter_element;

	QDir dir(m_szDirname);

	for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {
		if(!(*type)->getLocalName().isEmpty() && ((*type)->getClassType() != Type::TypeUnknown)) {
			buildHeaderFile(*type);
		}
	}

	for(iter_element = m_pListElement->constBegin(); iter_element != m_pListElement->constEnd(); ++iter_element)
	{
		RequestResponseElementSharedPtr pElement = (*iter_element);
		if(!pElement->getLocalName().isEmpty()) {
			buildHeaderFile(pElement);
		}
	}

	buildHeaderFile(m_pService);
}

void TypeListBuilder::buildCppFiles()
{
	TypeList::const_iterator type;
	RequestResponseElementList::const_iterator element;

	for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {
		if(!(*type)->getLocalName().isEmpty() == ((*type)->getClassType() != Type::TypeUnknown)) {
			buildCppFile(*type);
		}
	}

	for(element = m_pListElement->constBegin(); element != m_pListElement->constEnd(); ++element) {
		if(!(*element)->getLocalName().isEmpty()) {
			buildCppFile(*element);
		}
	}

	buildCppFile(m_pService);
}

void TypeListBuilder::buildFileDescription(QTextStream& os, const QString& szFilename)
{
	os << "/*" << CRLF;
	os << " * " << szFilename << CRLF;
	os << " * " << CRLF;
	os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
	//os << " * Created on: 16 sept. 2021" << CRLF;
	os << " * Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
	os << " * License: MIT License" << CRLF;
	os << " */" << CRLF;
}

void TypeListBuilder::buildHeaderFile(const TypeSharedPtr& pType)
{
	QString szHeaderFilename = getHeaderFileName(pType);
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, pType->getNamespace(), "types", szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pType->getNamespace(), "types", szHeaderFilename);

	// Remove existing file
	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	// Write the file
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = getDefine(m_szPrefix, pType);

		buildHeaderFileDescription(os, szHeaderFilename, szDefine);

		buildHeaderIncludeType(os, pType);

		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassType(os, pType);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szHeaderFilename = getHeaderFileName(pElement);
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, pElement->getNamespace(), "messages", szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pElement->getNamespace(), "messages", szHeaderFilename);

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = getDefine(m_szPrefix, pElement);

		buildHeaderFileDescription(os, szHeaderFilename, szDefine);

		buildHeaderIncludeElement(os, pElement);

		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassElement(os, pElement);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const ServiceSharedPtr& pService)
{
	QString szHeaderFilename = pService->getName() + ".h";
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, QString(), QString(), szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), QString(), QString(), szHeaderFilename);

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = (!m_szPrefix.isEmpty() ? m_szPrefix.toUpper() + "_" : "") + pService->getName().toUpper() + "_H_";

		buildHeaderFileDescription(os, szHeaderFilename, szDefine);

		buildHeaderIncludeService(os, m_pService);

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassService(os, m_pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFileDescription(QTextStream& os, const QString& szFilename, const QString& szDefine)
{
	buildFileDescription(os, szFilename);
	os << CRLF;
	os << "#ifndef " << szDefine << CRLF;
	os << "#define " << szDefine << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppFile(const TypeSharedPtr& pType)
{
	QString szHeaderFilename = getHeaderFileName(pType);
	QString szCppFilename = getCppFileName(pType);
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, pType->getNamespace(), "types", szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pType->getNamespace(), "types", szCppFilename);

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		buildCppFileDescription(os, szCppFilename);

		buildTypeIncludes(os, pType);

		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassType(os, pType);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szHeaderFilename = getHeaderFileName(pElement);
	QString szCppFilename = getCppFileName(pElement);
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, pElement->getNamespace(), "messages", szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pElement->getNamespace(), "messages", szCppFilename);

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		buildCppFileDescription(os, szCppFilename);

		buildTypeIncludes(os, pElement->getType());

		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassElement(os, pElement);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const ServiceSharedPtr& pService)
{
	QString szHeaderFilename = pService->getName() + ".h";
	QString szCppFilename = pService->getName() + ".cpp";
	QString szFullFilePath = FileHelper::buildPath(m_szDirname, QString(), QString(), szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), QString(), QString(), szCppFilename);

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		buildCppFileDescription(os, szCppFilename);

		os << "#include <QUrl>" << CRLF;
		os << "#include <QEventLoop>" << CRLF;
		os << "#include <QDomElement>" << CRLF;
		os << "#include <QNetworkAccessManager>" << CRLF;
		os << "#include <QNetworkRequest>" << CRLF;
		os << "#include <QNetworkReply>" << CRLF;
		os << "#include <QStringList>" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;
		os << "#define TIMEOUT_MSEC 10*1000" << CRLF;
		os << CRLF;
		os << "#define SOAP_ENV_URI \"http://www.w3.org/2003/05/soap-envelope\"" << CRLF;
		os << "#define DEFAULT_SOAP_ENV_NAMESPACE \"SOAP-ENV\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassService(os, pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pListGeneratedFiles->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFileDescription(QTextStream& os, const QString& szFilename)
{
	buildFileDescription(os, szFilename);
	os << CRLF;
}

void TypeListBuilder::buildHeaderClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
		os << CRLF;
	}

	if(pType->getClassType() == Type::TypeSimple) {
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

		buildHeaderClassSimpleType(os, pSimpleType);

		os << "};" << CRLF;
		os << CRLF;

	} else if(pType->getClassType() == Type::TypeComplex) {
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
		buildHeaderClassComplexType(os, pComplexType);
		endCppClass(os);
	}

	if(!szNamespace.isEmpty()){
		os << "}" << CRLF;
	}
}

void TypeListBuilder::buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
	os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
	os << "\t" << pSimpleType->getSerializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getDeserializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getEnumConvertDeclaration() << CRLF;
	os << "\t" << pSimpleType->getIsNullDeclaration() << CRLF;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\t" << pSimpleType->getVariableDeclaration() << CRLF;
}

void TypeListBuilder::buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
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

void TypeListBuilder::buildHeaderClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassName =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName(true);
	TypeSharedPtr pType = pElement->getType();
	QString szNamespace = StringUtils::secureString(pElement->getNamespace().toUpper());

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
		os << CRLF;
	}

	ComplexTypeSharedPtr pComplexType;
	if(pType->getClassType() == Type::TypeComplex){
		pComplexType = qSharedPointerCast<ComplexType>(pType);
	}
	if(!pComplexType.isNull())
	{
		// Write namespace
		os << "const QString " << szClassName << "TargetNamespace = \"" << pElement->getNamespace() << "\";" CRLF;
		os << "const QString " << szClassName << "TargetNamespaceUri = \"" << pElement->getNamespaceUri() << "\";" CRLF;
		os << CRLF;

		startCppClass(os, szClassName, pComplexType);
		buildHeaderClassComplexType(os, pComplexType);
		endCppClass(os);
	}

	if(!szNamespace.isEmpty()){
		os << "}" << CRLF;
	}
}

void TypeListBuilder::buildHeaderClassService(QTextStream& os, const ServiceSharedPtr& pService) const
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

void TypeListBuilder::buildHeaderIncludeType(QTextStream& os, const TypeSharedPtr& pType) const
{
	os << "#include <QDomElement>" << CRLF;
	os << "#include <QList>" << CRLF;
	os << "#include <QString>" << CRLF;
	os << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		if(pSimpleType->hasVariableType()) {
			os << "#include \"" << getSimpleTypeHeaderPath(pSimpleType, FileCategory_Type) << "\"" << CRLF;
		}
	}else if(pType->getClassType() == Type::TypeComplex){

		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
		AttributeList::const_iterator attr;
		AttributeSharedPtr pAttribute;

		ElementListSharedPtr pListElements = pComplexType->getElementList();
		ElementList::const_iterator element;
		ElementSharedPtr pElement;

		if(!pComplexType->getExtensionType().isNull()) {
			os << "#include \"" << getTypeHeaderPath(pComplexType->getExtensionType(), FileCategory_Type) << "\"" << CRLF;
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
				QString szFilename = getTypeHeaderPath(pAttribute->getType(), FileCategory_Type);
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
				QString szFilename = getTypeHeaderPath(pElement->getType(), FileCategory_Type);
				if(!szFilename.isEmpty() && !list.contains(szFilename)) {
					os << "#include \"" << szFilename << "\"" << CRLF;
					list.append(szFilename);
				}
			}
		}
	}
}

void TypeListBuilder::buildHeaderIncludeElement(QTextStream& os, const RequestResponseElementSharedPtr& pRequestResponseElement) const
{
	TypeSharedPtr pType = pRequestResponseElement->getType();

	ComplexTypeSharedPtr pComplexType;
	if(pType->getClassType() == Type::TypeComplex){
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
			os << "#include \"" << getTypeHeaderPath(pComplexType->getExtensionType(), FileCategory_Type) << "\"" << CRLF;
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
			QString szFilename = getTypeHeaderPath(pAttribute->getType(), FileCategory_Type);
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
			QString szFilename = getTypeHeaderPath(pElement->getType(), FileCategory_Message);
			if(!list.contains(szFilename)) {
				os << "#include \"" << szFilename << "\"" << CRLF;
				list.append(szFilename);
			}
		}
	}
}

void TypeListBuilder::buildHeaderIncludeService(QTextStream& os, const ServiceSharedPtr& pService) const
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
		if(pMessage){
			szTmpFileName = getRequestResponseElementHeaderPath(pMessage->getParameter(), FileCategory_Service);
			if(!list.contains(szTmpFileName)) {
				os << "#include \"" << szTmpFileName << "\"" << CRLF;
				list.append(szTmpFileName);
			}
		}

		pMessage = pOperation->getOutputMessage();
		szTmpFileName = getRequestResponseElementHeaderPath(pMessage->getParameter(), FileCategory_Service);
		if(!list.contains(szTmpFileName)) {
			os << "#include \"" << szTmpFileName << "\"" << CRLF;
			list.append(szTmpFileName);
		}
	}

	os << CRLF;
}

void TypeListBuilder::buildCppClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);
	QString szNamespace = StringUtils::secureString(pType->getNamespace().toUpper());

	if(!szNamespace.isEmpty()){
		os << "namespace " << szNamespace << " {" << CRLF;
		os << CRLF;
	}

	if(pType->getClassType() == Type::TypeSimple) {
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

		buildCppClassSimpleType(os, pSimpleType);

		os << CRLF;

	} else if(pType->getClassType() == Type::TypeComplex) {
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
			if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
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

		buildCppClassComplexType(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	if(!szNamespace.isEmpty()){
		os << "}" << CRLF;
		os << CRLF;
	}
}

void TypeListBuilder::buildCppClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
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

void TypeListBuilder::buildCppClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace) const
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

void TypeListBuilder::buildCppClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName();
	TypeSharedPtr pType = pElement->getType();
	ComplexTypeSharedPtr pComplexType;
	if(pType->getClassType() == Type::TypeComplex){
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
		os << CRLF;
	}

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		buildCppClassComplexType(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	if(!szNamespace.isEmpty()){
		os << "}" << CRLF;
		os << CRLF;
	}
}

void TypeListBuilder::buildCppClassService(QTextStream& os, const ServiceSharedPtr& pService) const
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

void TypeListBuilder::startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const
{
	os << "class " << szClassName;
	if(!pComplexType->getExtensionType().isNull())
	{
		QString szExtendedClassname;
		if(pComplexType->isExtensionTypeList()){
			szExtendedClassname = "QList<";
		}
		if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
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

void TypeListBuilder::endCppClass(QTextStream& os) const
{
	os << "};" << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildTypeIncludes(QTextStream& os, const TypeSharedPtr& pType)
{
	bool bHasIncludes = false;
	bool bQStringListIncluded = false;

	if(!pType){
		return;
	}

	if(pType->getClassType() == Type::TypeComplex)
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
					os << "#include \"" << getTypeHeaderPath((*iter)->getType(), FileCategory_Type) << "\"" << CRLF;
					bHasIncludes = true;
				}
			}
		}

		if(bHasIncludes){
			os << CRLF;
		}
	}
}
