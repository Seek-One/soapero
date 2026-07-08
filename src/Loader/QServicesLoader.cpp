//
// Created by ebeuque on 08/07/2026.
//

#include <QDir>

#include "Loader/QWSDLLoader.h"

#include "QServicesLoader.h"

QServicesLoader::QServicesLoader()
{
	m_iSuccessCount = 0;
	m_iWSDLFilesCount = 0;
}

QServicesLoader::~QServicesLoader()
{

}

void QServicesLoader::setSourceDirectory(const QString &szSourceDirectory)
{
	m_szSourceDirectory = szSourceDirectory;
}

void QServicesLoader::setServiceName(const QString &szServiceName)
{
	m_szServiceName = szServiceName;
}

void QServicesLoader::setNamespace(const QString &szNamespace)
{
	m_szNamespace = szNamespace;
}

void QServicesLoader::setResourcePath(const QString &szResourcePath)
{
	m_szResourcePath = szResourcePath;
}

void QServicesLoader::setOutputDirectory(const QString &szOutputDirectory)
{
	m_szOutputDirectory = szOutputDirectory;
}

QStringList QServicesLoader::getWSDLFileNames(const QString& szPath)
{
	QStringList listFileNames;
	QDir directory(szPath);
	if(directory.exists()){
		listFileNames = directory.entryList(QStringList() << "*.wsdl" << "*.WSDL" << "*.xsd" << "*.XSD", QDir::Files);
	}
	return listFileNames;
}

bool QServicesLoader::load(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	bool bGoOn = true;

	m_iSuccessCount = 0;
	m_iWSDLFilesCount = 0;

	QDir dirWDSLFiles(m_szSourceDirectory);
	QStringList listWSDLFileNames = getWSDLFileNames(m_szSourceDirectory);
	m_iWSDLFilesCount = (int)listWSDLFileNames.count();
	qDebug("[Main] %d WSDL files have been found in directory '%s'", (int)listWSDLFileNames.count(), qPrintable(m_szSourceDirectory));

	// Iterate over each WSDL files
	for(const auto& szFileName : listWSDLFileNames)
	{
		QString szFilePath = dirWDSLFiles.filePath(szFileName);

		QWSDLLoader loader;
		loader.setFileName(szFileName);
		loader.setFilePath(szFilePath);
		loader.setServiceName(m_szServiceName);
		loader.setNamespace(m_szNamespace);
		loader.setResourcePath(m_szResourcePath);
		loader.setOutputDirectory(m_szOutputDirectory);
		bGoOn = loader.processWSDLFile(bFileGenerated, pListGeneratedFiles);
		if (bGoOn) {
			m_iSuccessCount++;
		}else{
			qDebug("[Main] Error to process: %s", qPrintable(szFileName));
			break;
		}
	}

	return bGoOn;
}

int QServicesLoader::getSuccessCount() const
{
	return m_iSuccessCount;
}

int QServicesLoader::getWSDLFilesCount() const
{
	return m_iWSDLFilesCount;
}