//
// Created by ebeuque on 08/07/2026.
//

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Utils/FileDownloader.h"
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

	QDir dirSource(m_szSourceDirectory);
	if (dirSource.exists("services.json")) {
		return loadFromServiceJSON(bFileGenerated, pListGeneratedFiles);
	}else {
		return loadFromFiles(bFileGenerated, pListGeneratedFiles);
	}
	return bGoOn;
}

bool QServicesLoader::loadFromServiceJSON(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	bool bRes = true;

	QDir dirSource(m_szSourceDirectory);
	QString szJsonFilePath = dirSource.filePath("services.json");

	QString szNamespace;
	QList<ServiceDescriptor> listServiceDescriptor;
	bRes = loadServiceDescriptorFromJSON(szJsonFilePath, szNamespace, listServiceDescriptor);

	// Iterate over each service
	if (bRes) {
		// If parameters namespace is not empty, override the loaded namspace
		if (m_szNamespace.isEmpty()) {
			szNamespace = m_szNamespace;
		}

		// Display count of files
		m_iWSDLFilesCount = listServiceDescriptor.count();
		qDebug("[Main] %d services have been found in services.json", m_iWSDLFilesCount);

		for (const auto& serviceDescriptor: listServiceDescriptor)
		{
			const auto& szServiceName = serviceDescriptor.getServiceName();
			const auto& szFileName = serviceDescriptor.getFileName();
			const auto& szURL = serviceDescriptor.getURL();

			if (szServiceName.isEmpty() || szURL.isEmpty() || szFileName.isEmpty()) {
				qDebug("[Main] Warning: Service missing 'name', 'url' or 'filename', skipping");
				continue;
			}

			// Download the file
			QString szFilePath = dirSource.filePath(szFileName);
			if (bRes) {
				bRes = FileDownloader::downloadFile(szURL, szFilePath);
			}


			qDebug("[Main] Processing service: %s (url: %s)", qPrintable(szServiceName), qPrintable(szURL));

			QWSDLLoader loader;
			loader.setFileName(szFileName);
			loader.setFilePath(szFilePath);
			loader.setOriginalURL(szURL);
			loader.setServiceName(szServiceName);
			loader.setNamespace(szNamespace);
			loader.setResourcePath(m_szResourcePath);
			loader.setOutputDirectory(m_szOutputDirectory);
			bRes = loader.processWSDLFile(bFileGenerated, pListGeneratedFiles);

			if (bRes) {
				m_iSuccessCount++;
			} else {
				qDebug("[Main] Error to process service: %s", qPrintable(szServiceName));
				break;
			}
		}
	}

	return bRes;
}

bool QServicesLoader::loadServiceDescriptorFromJSON(const QString& szJsonFilePath, QString& szNamespace, QList<ServiceDescriptor>& listServiceDescriptor)
{
	bool bRes = true;

	QDir dirSource(m_szSourceDirectory);

	QFile jsonFile(szJsonFilePath);
	if (!jsonFile.open(QIODevice::ReadOnly)) {
		qDebug("[Main] Error: Cannot open services.json file at '%s'", qPrintable(szJsonFilePath));
		return false;
	}

	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
	if (jsonDoc.isNull() || !jsonDoc.isObject()) {
		qDebug("[Main] Error: Invalid JSON format in services.json");
		return false;
	}

	QJsonObject jsonRoot = jsonDoc.object();

	// Extract namespace if present
	if (jsonRoot.contains("namespace") && jsonRoot["namespace"].isString()) {
		szNamespace = jsonRoot["namespace"].toString();
		qDebug("[Main] Using namespace from JSON: %s", qPrintable(szNamespace));
	}

	// Extract services array
	if (!jsonRoot.contains("services") || !jsonRoot["services"].isArray()) {
		qDebug("[Main] Error: 'services' array not found in services.json");
		return false;
	}

	// Iterate over each service
	QJsonArray servicesArray = jsonRoot["services"].toArray();
	for (const QJsonValue &serviceValue: servicesArray) {
		if (!serviceValue.isObject()) {
			qDebug("[Main] Warning: Skipping invalid service entry");
			continue;
		}

		QJsonObject serviceObj = serviceValue.toObject();

		QString szServiceName = serviceObj.value("name").toString();
		QString szURL = serviceObj.value("url").toString();
		QString szFileName = serviceObj.value("filename").toString();


		qDebug("[Main] Processing service: %s (file: %s)", qPrintable(szServiceName), qPrintable(szFileName));

		ServiceDescriptor serviceDescriptor;
		serviceDescriptor.setServiceName(szServiceName);
		serviceDescriptor.setURL(szURL);
		serviceDescriptor.setFileName(szFileName);
		listServiceDescriptor.append(serviceDescriptor);
	}

	return bRes;
}

bool QServicesLoader::loadFromFiles(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	bool bRes = true;

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
		bRes = loader.processWSDLFile(bFileGenerated, pListGeneratedFiles);
		if (bRes) {
			m_iSuccessCount++;
		}else{
			qDebug("[Main] Error to process: %s", qPrintable(szFileName));
			break;
		}
	}

	return bRes;
}

int QServicesLoader::getSuccessCount() const
{
	return m_iSuccessCount;
}

int QServicesLoader::getWSDLFilesCount() const
{
	return m_iWSDLFilesCount;
}