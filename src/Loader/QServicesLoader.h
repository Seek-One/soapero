//
// Created by ebeuque on 08/07/2026.
//

#ifndef SOAPERO_QSERVICESLOADER_H
#define SOAPERO_QSERVICESLOADER_H

#include <QString>
#include <QSharedPointer>

#include "Utils/UniqueStringList.h"

class QServicesLoader
{
public:
	QServicesLoader();
	virtual ~QServicesLoader();

	void setSourceDirectory(const QString& szSourceDirectory);
	void setServiceName(const QString& szServiceName);
	void setNamespace(const QString& szNamespace);
	void setResourcePath(const QString& szResourcePath);
	void setOutputDirectory(const QString& szOutputDirectory);

	bool load(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles);

	int getSuccessCount() const;
	int getWSDLFilesCount() const;

	static QStringList getWSDLFileNames(const QString& szPath);

private:
	QString m_szSourceDirectory;
	QString m_szServiceName;
	QString m_szNamespace;
	QString m_szResourcePath;
	QString m_szOutputDirectory;

	int m_iSuccessCount;
	int m_iWSDLFilesCount;
};

#endif //SOAPERO_QSERVICESLOADER_H