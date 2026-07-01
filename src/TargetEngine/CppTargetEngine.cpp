//
// Created by ebeuque on 29/06/2026.
//

#include <QCoreApplication>

#include "Builder/FileHelper.h"
#include "Utils/StringUtils.h"

#include "LanguageWriter/CppWriter.h"

#include "CppTargetEngine.h"

CppTargetEngine::CppTargetEngine()
{

}

CppTargetEngine::~CppTargetEngine()
{

}

QString CppTargetEngine::getHeaderPath(const QString& szNamespace, const QString& szCategory, const QString& szFilename, FileCategory iOrigin)
{
	QString szRootPath;
	if(iOrigin == FileCategory_Type || iOrigin == FileCategory_Message)
	{
		szRootPath = "../../";
	}
	return FileHelper::buildPath(szRootPath, szNamespace, szCategory, szFilename);
}

void CppTargetEngine::doWriteFileDescription(QTextStream& os, const QString& szFilename) const
{
	os << "/*" << CRLF;
	os << " * " << szFilename << CRLF;
	os << " * " << CRLF;
#ifndef USE_COMPAT_TEST
	os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
#else
	os << " * Created on: 29 Jun 2026" << CRLF;
#endif
	os << " * Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
	os << " * License: MIT License" << CRLF;
	os << " */" << CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteNamespaceTargetInfos(QTextStream& os, const QString& szPrefix, const QString& szNamespace, const QString& szNamespaceURI) const
{
	os << CRLF;
	os << "const char* " << szPrefix << "_TargetNamespace = \"" << szNamespace << "\";" CRLF;
	os << "const char* " << szPrefix << "_TargetNamespaceUri = \"" << szNamespaceURI << "\";" CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteDeclarationClassStart(QTextStream& os, const QString& szClassName) const
{
	doWriteDeclarationClassStart(os, szClassName, QString());
}

void CppTargetEngine::doWriteDeclarationClassStart(QTextStream& os, const QString& szClassName, const QString& szBaseClass) const
{
	os << "class " << szClassName;
	if (!szBaseClass.isEmpty()) {
		os << " : public " << szBaseClass;
	}
	os << CRLF;
	os << "{" << CRLF;
}

void CppTargetEngine::doWriteDeclarationClassEnd(QTextStream& os, const QString& szClassName) const
{
	os << "};" << CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteDeclarationClassInitializers(QTextStream& os, const QString& szClassName, bool bEnumeration) const
{
	if (!bEnumeration) {
		os << "public:" << CRLF;
	}
	os << "\t" << szClassName << "();" << CRLF;
	os << "\tvirtual ~" << szClassName << "();" << CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteDeclarationGetter(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const
{
	QString szDeclaration;
	if(iReturnMode == GetterReturnModePointer) {
		szDeclaration = "%0* get%1() const;";
	}else if(iReturnMode == GetterReturnModeConst) {
		szDeclaration = "const %0& get%1() const;";
	}else{
		szDeclaration = "%0 get%1() const;";
	}
	szDeclaration = szDeclaration.arg(szMemberType).arg(szFuncName);
	os << "\t" << szDeclaration << CRLF;
}

void CppTargetEngine::doWriteDeclarationGetterList(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const
{
	QString szDeclaration;
	szDeclaration = "const std::list<%0>& get%1List() const;";
	szDeclaration = szDeclaration.arg(szMemberType).arg(szFuncName);
	os << "\t" << szDeclaration << CRLF;
}

void CppTargetEngine::doWriteDeclarationSetter(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const
{
	QString szDeclaration;
	if(iParamMode == SetterParamModeConst) {
		szDeclaration = "void set%0(const %1& %2);";
	}else if(iParamMode == SetterParamModePointer) {
		szDeclaration = "void set%0(%1* %2);";
	}else{
		szDeclaration = "void set%0(%1 %2);";
	}
	szDeclaration = szDeclaration.arg(szFuncName).arg(szParamType).arg(szParamName);
	os << "\t" << szDeclaration << CRLF;
}

void CppTargetEngine::doWriteDeclarationSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const
{
	QString szDeclaration;
	szDeclaration = "void set%0List(const std::list<%1>& %2List);";
	szDeclaration = szDeclaration.arg(szFuncName).arg(szParamType).arg(szParamName);
	os << "\t" << szDeclaration << CRLF;
}

void CppTargetEngine::doWriteDeclarationAddList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const
{
	QString szDeclaration;
	if (iParamMode == SetterParamModePointer) {
		szDeclaration += "void add%0(%1* %2);";
	}else {
		szDeclaration += "void add%0(const %1& %2);";
	}
	szDeclaration = szDeclaration.arg(szFuncName).arg(szParamType).arg(szParamName);
	os << "\t" << szDeclaration << CRLF;
}

void CppTargetEngine::doWriteDefinitionGetter(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const
{
	QString szDefinition;
	if(iReturnMode == GetterReturnModePointer) {
		szDefinition = "%0* %1::get%2() const" CRLF;
	}else if(iReturnMode == GetterReturnModeConst) {
		szDefinition = "const %0& %1::get%2() const" CRLF;
	}else{
		szDefinition = "%0 %1::get%2() const" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\treturn %3;" CRLF;
	szDefinition += "}" CRLF;;
	szDefinition =  szDefinition.arg(szMemberType).arg(szClassName).arg(szFuncName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void CppTargetEngine::doWriteDefinitionGetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const
{
	QString szDefinition;
	szDefinition += "const std::list<%0>& %1::get%2List() const" CRLF;
	szDefinition += "{" CRLF;
	szDefinition += "\treturn %3;" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szMemberType).arg(szClassName).arg(szFuncName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void CppTargetEngine::doWriteDefinitionSetter(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const
{
	QString szDefinition;
	if(iParamMode == SetterParamModeConst) {
		szDefinition = "void %0::set%1(const %2& %3)" CRLF;
	}else if(iParamMode == SetterParamModePointer) {
		szDefinition = "void %0::set%1(%2* %3)" CRLF;
	}else{
		szDefinition = "void %0::set%1(%2 %3)" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\t%4 = %3;" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(szParamType).arg(szParamName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void CppTargetEngine::doWriteDefinitionSetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const
{
	QString szDefinition;
	szDefinition += "void %0::set%1List(const std::list<%2>& %3List)" CRLF;
	szDefinition += "{" CRLF;
	szDefinition += "\t%4 = %3List;" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(szParamType).arg(szParamName).arg(szMemberName);
	os << szDefinition << CRLF;
}

void CppTargetEngine::doWriteDefinitionAddList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const
{
	QString szDefinition;
	szDefinition += "void %0::add%1(const %2& %3)" CRLF;
	szDefinition += "{" CRLF;
	szDefinition += "\t%4.push(%3);" CRLF;
	szDefinition += "}" CRLF;
	szDefinition = szDefinition.arg(szClassName).arg(szFuncName).arg(szParamType).arg(szParamName).arg(szMemberName);
	os << szDefinition << CRLF;
}

//////////////////
// Service files
//////////////////

QString CppTargetEngine::getHeaderGuard(const QString& szPrefix, const ServiceSharedPtr& pService)
{
	QString szServiceName = pService->getName();

	QString szHeaderGuard;
	if(!szPrefix.isEmpty()){
		szHeaderGuard += StringUtils::secureString(szPrefix).toUpper() + "_";
	}

#ifdef WITH_DIR_CREATION
	if(!szServiceName.isEmpty()){
		szHeaderGuard += StringUtils::secureString(szServiceName).toUpper() + "_";
	}
#else
	szHeaderGuard += StringUtils::secureString(szServiceName).toUpper() + "_";
#endif

	szHeaderGuard += "H_";

	return szHeaderGuard;
}

bool CppTargetEngine::doBuildService(const ServiceSharedPtr& pService)
{
	bool bRes = true;
	bRes = doBuildHeaderFile(pService) && bRes;
	bRes = doBuildCppFile(pService) && bRes;
	return bRes;
}

bool CppTargetEngine::doBuildHeaderFile(const ServiceSharedPtr& pService)
{
	bool bRes = false;

	QString szHeaderFilename = pService->getName() + ".h";
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, QString(), QString(), szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), QString(), QString(), szHeaderFilename);

	qDebug("[Builder] Creating file: %s", qPrintable(szFullFilePath));

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pService);
		langWriter.writeDeclarationGuardStart(szHeaderGuard);
		// Includes
		doWriteDeclarationIncludes(os, m_pService);
		// Namespace start
		langWriter.writeNamespaceStart(m_szNamespace);
		// Class
		doWriteDeclarationClass(os, m_pService);
		// Namespace end
		langWriter.writeNamespaceEnd(m_szNamespace);
		// Header guard end
		langWriter.writeDeclarationGuardEnd(szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}else{
		qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

bool CppTargetEngine::doBuildCppFile(const ServiceSharedPtr& pService)
{
	bool bRes = false;

	QString szHeaderFilename = pService->getName() + ".h";
	QString szCppFilename = pService->getName() + ".cpp";
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, QString(), QString(), szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), QString(), QString(), szCppFilename);

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szCppFilename);

		langWriter.writeIncludeFileSystem("QUrl");
		langWriter.writeIncludeFileSystem("QEventLoop");
		langWriter.writeIncludeFileSystem("QDomElement");
		langWriter.writeIncludeFileSystem("QNetworkAccessManager");
		langWriter.writeIncludeFileSystem("QNetworkRequest");
		langWriter.writeIncludeFileSystem("QNetworkReply");
		langWriter.writeIncludeFileSystem("QStringList");
		os << CRLF;
		langWriter.writeIncludeFileLocal(szHeaderFilename);
		os << CRLF;
		os << "#define TIMEOUT_MSEC 10*1000" << CRLF;
		os << CRLF;
		os << "#define SOAP_ENV_URI \"http://www.w3.org/2003/05/soap-envelope\"" << CRLF;
		os << "#define DEFAULT_SOAP_ENV_NAMESPACE \"SOAP-ENV\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		doWriteDefinitionClass(os, pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}else{
		qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const ServiceSharedPtr& pService) const
{

}

void CppTargetEngine::doWriteDeclarationClass(QTextStream& os, const ServiceSharedPtr& pService) const
{

}

void CppTargetEngine::doWriteDefinitionClass(QTextStream& os, const ServiceSharedPtr& pService) const
{

}

//////////////////////////////////
// Request response elements files
//////////////////////////////////

QString CppTargetEngine::getHeaderFileName(const RequestResponseElementSharedPtr& pElement)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pElement->getLocalName();
#else
	szClassName = pElement->getQualifiedName();
#endif
	return FileHelper::buildFileName(pElement->getNamespace(), szClassName, "h");
}

QString CppTargetEngine::getHeaderFilePath(const RequestResponseElementSharedPtr& pRequestResponseElement, FileCategory iOrigin)
{
	QString szFileName = getHeaderFileName(pRequestResponseElement);
	return getHeaderPath( pRequestResponseElement->getNamespace(), "messages", szFileName, iOrigin);
}

QString CppTargetEngine::getCppFileName(const RequestResponseElementSharedPtr& pElement)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pElement->getLocalName();
#else
	szClassName = pElement->getQualifiedName();
#endif
	return FileHelper::buildFileName(pElement->getNamespace(), szClassName, "cpp");
}

QString CppTargetEngine::getHeaderGuard(const QString& szPrefix, const RequestResponseElementSharedPtr& pElement)
{
	QString szHeaderGuard;

	if(!szPrefix.isEmpty()){
		szHeaderGuard += StringUtils::secureString(szPrefix).toUpper() + "_";
	}

#ifdef WITH_DIR_CREATION
	if(!pElement->getNamespace().isEmpty()){
		szHeaderGuard += StringUtils::secureString(pElement->getNamespace()).toUpper() + "_";
	}
	szHeaderGuard += "MSG_";
	if(!pElement->getLocalName().isEmpty()){
		szHeaderGuard += StringUtils::secureString(pElement->getLocalName()).toUpper() + "_";
	}
#else
	szDefine += StringUtils::secureString(pElement->getQualifiedName()).toUpper() + "_";
#endif

	szHeaderGuard += "H_";

	return szHeaderGuard;
}

bool CppTargetEngine::doBuildRequestResponseElement(const RequestResponseElementSharedPtr& pElement)
{
	bool bRes = true;
	bRes = doBuildHeaderFile(pElement) && bRes;
	bRes = doBuildCppFile(pElement) && bRes;
	return bRes;
}

bool CppTargetEngine::doBuildHeaderFile(const RequestResponseElementSharedPtr& pElement)
{
	bool bRes = false;

	QString szHeaderFilename = getHeaderFileName(pElement);
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, pElement->getNamespace(), "messages", szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pElement->getNamespace(), "messages", szHeaderFilename);

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pElement);
		langWriter.writeDeclarationGuardStart(szHeaderGuard);
		// Includes
		doWriteDeclarationIncludes(os, pElement);
		// Namespace start
		langWriter.writeNamespaceStart(m_szNamespace);
		// Class
		doWriteDeclarationClass(os, pElement);
		// Namespace end
		langWriter.writeNamespaceEnd(m_szNamespace);
		// Header guard end
		langWriter.writeDeclarationGuardEnd(szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}else{
		qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

bool CppTargetEngine::doBuildCppFile(const RequestResponseElementSharedPtr& pElement)
{
	bool bRes = false;

	QString szHeaderFilename = getHeaderFileName(pElement);
	QString szCppFilename = getCppFileName(pElement);
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, pElement->getNamespace(), "messages", szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pElement->getNamespace(), "messages", szCppFilename);

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szCppFilename);
		// Includes
		doWriteDefinitionIncludes(os, pElement->getType());
		// Add related header
		langWriter.writeIncludeFileLocal(szHeaderFilename);
		os << CRLF;
		// Namespace start
		langWriter.writeNamespaceStart(m_szNamespace);
		// Class
		doWriteDefinitionClass(os, pElement);
		// Namespace end
		langWriter.writeNamespaceEnd(m_szNamespace);

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}else{
		qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{

}

void CppTargetEngine::doWriteDeclarationClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{

}

void CppTargetEngine::doWriteDefinitionClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const {

}

//////////////////////////////////
// Types files
//////////////////////////////////

QString CppTargetEngine::getHeaderFileName(const TypeSharedPtr& pType)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pType->getLocalName();
#else
	szClassName = pType->getQualifiedName();
#endif
	return FileHelper::buildFileName(pType->getNamespace(), szClassName, "h");
}

QString CppTargetEngine::getHeaderFilePath(const TypeSharedPtr& pType, FileCategory iOrigin)
{
	if(pType->getTypeMode() == Type::TypeSimple){
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);
		return getHeaderFilePath(pSimpleType, iOrigin);
	}else if(pType->getTypeMode() == Type::TypeComplex){
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
		return getComplexTypeHeaderPath(pComplexType, iOrigin);
	}else{
		qWarning("[Builder] Unknown type: %s", qPrintable(pType->getLocalName()));
		QString szFileName = getHeaderFileName(pType);
		return getHeaderPath(pType->getNamespace(), "types", szFileName, iOrigin);
	}
}

QString CppTargetEngine::getCppFileName(const TypeSharedPtr& pType)
{
	QString szClassName;
#ifdef WITH_DIR_CREATION
	szClassName = pType->getLocalName();
#else
	szClassName = pType->getQualifiedName();
#endif
	return FileHelper::buildFileName(pType->getNamespace(), szClassName, "cpp");
}

QString CppTargetEngine::getHeaderGuard(const QString& szPrefix, const TypeSharedPtr& pType)
{
	QString szHeaderGuard;

	if(!szPrefix.isEmpty()){
		szHeaderGuard += StringUtils::secureString(szPrefix).toUpper() + "_";
	}

#ifdef WITH_DIR_CREATION
	if(!pType->getNamespace().isEmpty()){
		szHeaderGuard += StringUtils::secureString(pType->getNamespace()).toUpper() + "_";
	}
	szHeaderGuard += "TYPES_";
	if(!pType->getLocalName().isEmpty()){
		szHeaderGuard += StringUtils::secureString(pType->getLocalName()).toUpper() + "_";
	}
#else
	szHeaderGuard += pType->getQualifiedName().toUpper() + "_";
#endif

	szHeaderGuard += "H_";

	return szHeaderGuard;
}

bool CppTargetEngine::doBuildType(const TypeSharedPtr& pType)
{
	bool bRes = true;
	bRes = doBuildHeaderFile(pType) && bRes;
	bRes = doBuildCppFile(pType) && bRes;
	return bRes;
}

bool CppTargetEngine::doBuildHeaderFile(const TypeSharedPtr& pType) {
	bool bRes = false;

	QString szHeaderFilename = getHeaderFileName(pType);
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, pType->getNamespace(), "types", szHeaderFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pType->getNamespace(), "types", szHeaderFilename);

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pType);
		langWriter.writeDeclarationGuardStart(szHeaderGuard);
		// Includes
		doWriteDeclarationIncludes(os, pType);
		// Namespace start
		langWriter.writeNamespaceStart(m_szNamespace);
		// Class
		doWriteDeclarationClass(os, pType);
		// Namespace end
		langWriter.writeNamespaceEnd(m_szNamespace);
		// Header guard end
		langWriter.writeDeclarationGuardEnd(szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}

	return bRes;
}

bool CppTargetEngine::doBuildCppFile(const TypeSharedPtr& pType)
{
	bool bRes = false;

	QString szHeaderFilename = getHeaderFileName(pType);
	QString szCppFilename = getCppFileName(pType);
	QString szFullFilePath = FileHelper::buildPath(m_outputDir, pType->getNamespace(), "types", szCppFilename);
	QString szShortFilePath = FileHelper::buildPath(QString(), pType->getNamespace(), "types", szCppFilename);

	// Write the file
	QFile file(szFullFilePath);
	bRes = openFile(file);
	if(bRes) {
		QTextStream os(&file);
		CppWriter langWriter(os);

		// File description
		doWriteFileDescription(os, szCppFilename);
		// Includes
		doWriteDefinitionIncludes(os, pType);
		// Add related header
		langWriter.writeIncludeFileLocal(szHeaderFilename);
		os << CRLF;
		// Namespace start
		langWriter.writeNamespaceStart(m_szNamespace);
		// Class
		doWriteDefinitionClass(os, pType);
		// Namespace end
		langWriter.writeNamespaceEnd(m_szNamespace);

		m_pGeneratedFilesList->append(szShortFilePath);

		closeFile(file);
	}else{
		qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteDeclarationIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteDeclarationClass(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteDefinitionIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteDefinitionClass(QTextStream& os, const TypeSharedPtr& pType) const
{

}

//////////////////////////////////
// Simple types files
//////////////////////////////////

QString CppTargetEngine::getHeaderFileName(const SimpleTypeSharedPtr& pSimpleType)
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

QString CppTargetEngine::getHeaderFilePath(const SimpleTypeSharedPtr& pSimpleType, FileCategory iOrigin)
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

QString CppTargetEngine::getSimpleTypeNamespace(const SimpleTypeSharedPtr& pSimpleType)
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

//////////////////////////////////
// Complex types files
//////////////////////////////////

QString CppTargetEngine::getComplexTypeHeaderPath(const ComplexTypeSharedPtr& pComplexType, FileCategory iOrigin)
{
	QString szFileName = getHeaderFileName((TypeSharedPtr)pComplexType);
	return getHeaderPath( pComplexType->getNamespace(), "types", szFileName, iOrigin);
}
