//
// Created by ebeuque on 29/06/2026.
//

#include "Builder/FileHelper.h"

#include "TargetEngine.h"

TargetEngine::TargetEngine()
{

}

TargetEngine::~TargetEngine()
{

}

void TargetEngine::setOutputDirectory(const QDir& dir)
{
	m_outputDir = dir;
}

void TargetEngine::setPrefix(const QString& szPrefix)
{
	m_szPrefix = szPrefix;
}

void TargetEngine::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

void TargetEngine::setService(const ServiceSharedPtr& pService)
{
	m_pService = pService;
}

void TargetEngine::setRequestResponseElementList(const RequestResponseElementListSharedPtr& pElementList)
{
	m_pElementList = pElementList;
}

void TargetEngine::setTypeList(const TypeListSharedPtr& pTypeList)
{
	m_pTypeList = pTypeList;
}

void TargetEngine::setGeneratedFileList(const QSharedPointer<UniqueStringList>& pGeneratedFilesList)
{
	m_pGeneratedFilesList = pGeneratedFilesList;
}

bool TargetEngine::build()
{
	return doBuild();
}

bool TargetEngine::doBuild()
{
	bool bRes = false;
	bRes = doBuildTypeList(m_pTypeList);
	bRes = doBuildRequestResponseElementList(m_pElementList) && bRes;
	bRes = doBuildService(m_pService) && bRes;
	return bRes;
}

bool TargetEngine::doBuildService(const ServiceSharedPtr& pService)
{
	// Implement in subclass
	return true;
}

bool TargetEngine::doBuildRequestResponseElementList(const RequestResponseElementListSharedPtr& pElementList)
{
	bool bRes = true;
	for(const auto& pElement :  *m_pElementList)
	{
		if(!pElement->getLocalName().isEmpty()) {
			bRes = doBuildRequestResponseElement(pElement) && bRes;
		}
	}
	return bRes;
}

bool TargetEngine::doBuildRequestResponseElement(const RequestResponseElementSharedPtr& pElement)
{
	// Implement in subclass
	return true;
}

bool TargetEngine::doBuildTypeList(const TypeListSharedPtr& pTypeList)
{
	bool bRes = true;
	for(const auto& pType :  *m_pTypeList)
	{
		if(!pType->getLocalName().isEmpty()) {
			bRes = doBuildType(pType) && bRes;
		}
	}
	return bRes;
}

bool TargetEngine::doBuildType(const TypeSharedPtr& pType)
{
	// Implement in subclass
	return true;
}

TypeSharedPtr TargetEngine::findTypeByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes) const
{
	if (m_pTypeList) {
		return m_pTypeList->getByName(szLocalName, szNamespace, pListIgnoredTypes);
	}
	return TypeSharedPtr();
}

bool TargetEngine::openFile(QFile& file, bool bCleanFirst) const
{
	bool bRes = true;

	QString szFilePath = file.fileName();

	qDebug("[Builder] Creating file: %s", qPrintable(szFilePath));

	// Remove existing file
	if(file.exists()) {
		bRes = file.remove();
		if (!bRes) {
			qCritical("[Builder] Error to remove file %s", qPrintable(szFilePath));
		}
	}

	if (bRes) {
		// Create directory for file
		bRes = FileHelper::createDirectoryForFile(szFilePath);
		if (!bRes) {
			qCritical("[Builder] Error to create file directory %s", qPrintable(szFilePath));
		}
	}

	if (bRes) {
		// Write the file
		bRes = file.open(QFile::WriteOnly);
		if (!bRes) {
			qWarning("[Builder] Cannot open file %s (error: %s)", qPrintable(szFilePath), qPrintable(file.errorString()));
		}
	}

	return bRes;
}

void TargetEngine::closeFile(QFile& file) const
{
	file.close();
}