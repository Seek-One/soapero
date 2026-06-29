//
// Created by ebeuque on 29/06/2026.
//

#include <QCoreApplication>

#include "Builder/FileHelper.h"
#include "Utils/StringUtils.h"

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

void CppTargetEngine::doWriteFileDescription(QTextStream& os, const QString& szFilename)
{
	os << "/*" << CRLF;
	os << " * " << szFilename << CRLF;
	os << " * " << CRLF;
	os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
	//os << " * Created on: 16 sept. 2021" << CRLF;
	os << " * Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
	os << " * License: MIT License" << CRLF;
	os << " */" << CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteHeaderGuardStart(QTextStream& os, const QString& szHeaderGuard)
{
	os << CRLF;
	os << "#ifndef " << szHeaderGuard << CRLF;
	os << "#define " << szHeaderGuard << CRLF;
	os << CRLF;
}

void CppTargetEngine::doWriteHeaderGuardEnd(QTextStream& os, const QString& szHeaderGuard)
{
	os << "#endif //" << szHeaderGuard << CRLF;
}

void CppTargetEngine::doWriteNamespaceStart(QTextStream& os, const QString& szNamespace)
{
	if(!szNamespace.isEmpty()) {
		os << "namespace " << szNamespace << " {" << CRLF;
	}
}

void CppTargetEngine::doWriteNamespaceEnd(QTextStream& os, const QString& szNamespace)
{
	if(!szNamespace.isEmpty()) {
		os << "} // " << szNamespace << CRLF;
	}
	os << CRLF;
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

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	bRes = file.open(QFile::WriteOnly);
	if(bRes) {

		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pService);
		doWriteHeaderGuardStart(os, szHeaderGuard);
		// Includes
		doWriteHeaderIncludes(os, m_pService);
		// Namespace start
		doWriteNamespaceStart(os, m_szNamespace);
		// Class
		doWriteHeaderClass(os, m_pService);
		// Namespace end
		doWriteNamespaceEnd(os, m_szNamespace);
		// Header guard end
		doWriteHeaderGuardEnd(os, szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
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

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	bRes = file.open(QFile::WriteOnly);
	if(bRes) {

		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szCppFilename);

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

		doWriteCppClass(os, pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const ServiceSharedPtr& pService) const
{

}

void CppTargetEngine::doWriteHeaderClass(QTextStream& os, const ServiceSharedPtr& pService) const
{

}

void CppTargetEngine::doWriteCppClass(QTextStream& os, const ServiceSharedPtr& pService) const
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

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	bRes = file.open(QFile::WriteOnly);
	if(bRes) {
		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pElement);
		doWriteHeaderGuardStart(os, szHeaderGuard);
		// Includes
		doWriteHeaderIncludes(os, pElement);
		// Namespace start
		doWriteNamespaceStart(os, m_szNamespace);
		// Class
		doWriteHeaderClass(os, pElement);
		// Namespace end
		doWriteNamespaceEnd(os, m_szNamespace);
		// Header guard end
		doWriteHeaderGuardEnd(os, szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
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

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	bRes = file.open(QFile::WriteOnly);
	if(bRes) {

		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szCppFilename);
		// Includes
		doWriteCppIncludes(os, pElement->getType());
		// Add related header
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;
		// Namespace start
		doWriteNamespaceStart(os, m_szNamespace);
		// Class
		doWriteCppClass(os, pElement);
		// Namespace end
		doWriteNamespaceEnd(os, m_szNamespace);

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{

}

void CppTargetEngine::doWriteHeaderClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{

}

void CppTargetEngine::doWriteCppClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const {

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
		qWarning("[TypeListBuilder] Unknown type: %s", qPrintable(pType->getLocalName()));
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

	// Remove existing file
	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	// Write the file
	bRes = file.open(QFile::WriteOnly);
	if(bRes) {

		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szHeaderFilename);
		// Header guard start
		QString szHeaderGuard = getHeaderGuard(m_szNamespace, pType);
		doWriteHeaderGuardStart(os, szHeaderGuard);
		// Includes
		doWriteHeaderIncludes(os, pType);
		// Namespace start
		doWriteNamespaceStart(os, m_szNamespace);
		// Class
		doWriteHeaderClass(os, pType);
		// Namespace end
		doWriteNamespaceEnd(os, m_szNamespace);
		// Header guard end
		doWriteHeaderGuardEnd(os, szHeaderGuard);

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
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

	QFile file(szFullFilePath);
	if(file.exists()) {
		file.remove();
	}

	// Create directory for file
	FileHelper::createDirectoryForFile(szFullFilePath);

	bRes = file.open(QFile::WriteOnly);
	if(bRes) {

		QTextStream os(&file);

		// File description
		doWriteFileDescription(os, szCppFilename);
		// Includes
		doWriteCppIncludes(os, pType);
		// Add related header
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;
		// Namespace start
		doWriteNamespaceStart(os, m_szNamespace);
		// Class
		doWriteCppClass(os, pType);
		// Namespace end
		doWriteNamespaceEnd(os, m_szNamespace);

		m_pGeneratedFilesList->append(szShortFilePath);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}

	return bRes;
}

void CppTargetEngine::doWriteHeaderIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteCppIncludes(QTextStream& os, const TypeSharedPtr& pType) const
{

}

void CppTargetEngine::doWriteCppClass(QTextStream& os, const TypeSharedPtr& pType) const
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
