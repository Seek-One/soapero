/*
 * Service.h
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#ifndef SOAPERO_SERVICE_H_
#define SOAPERO_SERVICE_H_

#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QUrl>

#include <QNetworkRequest>

// Added in Qt 5.15.0
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define USE_QRANDOMGENERATOR
#endif

// Added in Qt 6.5.0
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
#define USE_QDOMDOCUMENT_PARSERRESULT
#endif

#ifdef USE_QRANDOMGENERATOR
#include <QRandomGenerator>
#endif

#include <QDomDocument>

namespace SOAPERO {

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

#ifdef USE_QDOMDOCUMENT_PARSERRESULT
typedef QDomDocument::ParseResult QDomDocumentParseResult;
#else
struct QDomDocumentParseResult {
	int errorColumn;
	int errorLine;
	QString errorMessage;
};
#endif

class Service
{
public:
	Service();
	virtual ~Service();

	void setBaseUrl(const QUrl& url);
	void setUseWSUsernameToken(bool bUseWSUsernameToken);
	void setCustomDateTime(const QDateTime& customDateTime);
	void setQueryExecutor(IQueryExecutor* pExecutor);

	int lastErrorCode() const;
	const QString& lastError() const;

protected:
	QNetworkRequest buildNetworkRequest() const;
	QByteArray buildSoapMessage(const QString& szSerializedObject, const QList<QString>& listNamespaceDeclaration) const;

	bool setContent(QDomDocument& doc, const QByteArray& bytesContent, QDomDocumentParseResult& parseResult);

	QUrl m_url;
	bool m_bUseWSUsernameToken;
	bool m_bUseCustomDateTime;
	QDateTime m_customDateTime;

	int m_iLastErrorCode;
	QString m_szLastError;

	IQueryExecutor* m_pQueryExecutor;

private:
	QString buildNonce() const;

#ifdef USE_QRANDOMGENERATOR
	mutable QRandomGenerator m_rand;
#endif
};

}


#endif /* SOAPERO_SERVICE_H_ */
