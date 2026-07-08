//
// Created by ebeuque on 08/07/2026.
//

#ifndef SOAPERO_SERVICEDESCRIPTOR_H
#define SOAPERO_SERVICEDESCRIPTOR_H

#include <QString>

class ServiceDescriptor
{
public:
	ServiceDescriptor();
	virtual ~ServiceDescriptor();

	void setServiceName(const QString& szServiceName);
	const QString& getServiceName() const;

	void setURL(const QString& szURL);
	const QString& getURL() const;

	void setFileName(const QString& szFileName);
	const QString& getFileName() const;

private:
	QString m_szServiceName;
	QString m_szURL;
	QString m_szFileName;
};


#endif //SOAPERO_SERVICEDESCRIPTOR_H