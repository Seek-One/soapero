/*
 * Service.h
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QString>
#include <QByteArray>
#include <QUrl>

#include <QNetworkRequest>

namespace Onvif {

class IQueryExecutor
{
public:
	IQueryExecutor();
	virtual ~IQueryExecutor();

	virtual QByteArray execQuery(const QNetworkRequest& request, const QByteArray& bytes) = 0;
};

class Service
{
public:
	Service();
	virtual ~Service();

	void setUrl(const QUrl& url);
	void setQueryExecutor(IQueryExecutor* pExecutor);

    int lastErrorCode() const;
    const QString& lastError() const;

protected:
	QNetworkRequest buildNetworkRequest() const;
	QByteArray buildSoapMessage(const QString& szSerializedObject) const;

	QUrl m_url;

	int m_iLastErrorCode;
	QString m_szLastError;

	IQueryExecutor* m_pQueryExecutor;

private:
	QString buildNonce() const;
};

}


#endif /* SERVICE_H_ */
