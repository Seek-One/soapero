/*
 * Service.h
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QUrl>

#include <QNetworkRequest>

namespace ONVIF {

class IQueryExecutorResponse
{
public:
	IQueryExecutorResponse();
	virtual ~IQueryExecutorResponse();

	void setResponse(const QByteArray& response);
	const QByteArray& getResponse() const;

	void setHttpStatusCode(int iHttpStatusCode);
	int getHttpStatusCode() const;

private:
	QByteArray m_response;
	int m_iHttpStatusCode;
};

class IQueryExecutor
{
public:
	IQueryExecutor();
	virtual ~IQueryExecutor();

	virtual IQueryExecutorResponse execQuery(const QNetworkRequest& request, const QByteArray& bytes) = 0;
};

class Service
{
public:
	Service();
	virtual ~Service();

	void setUrl(const QUrl& url);
	void setUseWSUsernameToken(bool bUseWSUsernameToken);
	void setCustomDateTime(const QDateTime& customDateTime);
	void setQueryExecutor(IQueryExecutor* pExecutor);

    int lastErrorCode() const;
    const QString& lastError() const;

protected:
	QNetworkRequest buildNetworkRequest() const;
	QByteArray buildSoapMessage(const QString& szSerializedObject, const QList<QString>& listNamespaceDeclaration) const;

	QUrl m_url;
	bool m_bUseWSUsernameToken;
	bool m_bUseCustomDateTime;
	QDateTime m_customDateTime;

	int m_iLastErrorCode;
	QString m_szLastError;

	IQueryExecutor* m_pQueryExecutor;

private:
	QString buildNonce() const;
};

}


#endif /* SERVICE_H_ */
