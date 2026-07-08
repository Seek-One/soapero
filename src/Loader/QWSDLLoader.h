//
// Created by ebeuque on 08/07/2026.
//

#ifndef SOAPERO_LOADER_QWSDLLOADER_H
#define SOAPERO_LOADER_QWSDLLOADER_H

#include <QString>
#include <QSharedPointer>

#include "Utils/UniqueStringList.h"

class QWSDLLoader
{
public:
	QWSDLLoader();
	virtual ~QWSDLLoader();

	// Input informations
	void setFileName(const QString& szFileName);
	void setFilePath(const QString& szFilePath);
	void setOriginalURL(const QString& szOriginalURL);
	void setServiceName(const QString& szServiceName);
	void setNamespace(const QString& szNamespace);
	void setResourcePath(const QString& szResourcePath);
	void setOutputDirectory(const QString& szOutputDirectory);

public:
	bool processWSDLFile(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles);

private:
	QString m_szFileName;
	QString m_szFilePath;
	QString m_szOriginalURL;
	QString m_szServiceName;
	QString m_szNamespace;
	QString m_szResourcePath;
	QString m_szOutputDirectory;
};

#endif //SOAPERO_LOADER_QWSDLLOADER_H