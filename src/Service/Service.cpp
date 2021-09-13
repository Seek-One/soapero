/*
 * Service.cpp
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#include <QCryptographicHash>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// Added in Qt 5.15.0
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define USE_QRANDOMGENERATOR
#endif

#ifdef USE_QRANDOMGENERATOR
#include <QRandomGenerator>
#endif

#include "Service.h"

namespace ONVIF {

IQueryExecutor::IQueryExecutor(){}
IQueryExecutor::~IQueryExecutor(){}

IQueryExecutorResponse::IQueryExecutorResponse()
{
	m_iHttpStatusCode = 0;
}

IQueryExecutorResponse::~IQueryExecutorResponse()
{

}

void IQueryExecutorResponse::setResponse(const QByteArray& response)
{
	m_response = response;
}

const QByteArray& IQueryExecutorResponse::getResponse() const
{
	return m_response;
}

void IQueryExecutorResponse::setHttpStatusCode(int iHttpStatusCode)
{
	m_iHttpStatusCode = iHttpStatusCode;
}

int IQueryExecutorResponse::getHttpStatusCode() const
{
	return m_iHttpStatusCode;
}

class CustomQueryExecutor : public IQueryExecutor
{
public:
	virtual IQueryExecutorResponse execQuery(const QNetworkRequest& request, const QByteArray& bytes)
	{
		IQueryExecutorResponse response;
		QNetworkAccessManager *manager = new QNetworkAccessManager();
		QNetworkReply *reply = manager->post(request, bytes);
		QEventLoop loop;
		QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
		loop.exec();

		QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		if(statusCode.isValid()){
			response.setHttpStatusCode(statusCode.toInt());
		}
		response.setResponse(reply->readAll());

		if(manager) {
			delete manager;
			manager = NULL;
		}

		if(reply) {
			reply->deleteLater();
		}

		return response;
	}
};

Service::Service()
{
	m_iLastErrorCode = -1;
	m_pQueryExecutor = new CustomQueryExecutor();
	m_bUseWSUsernameToken = false;
	m_bUseCustomDateTime = false;
}

Service::~Service()
{
	if(m_pQueryExecutor) {
		delete m_pQueryExecutor;
		m_pQueryExecutor = NULL;
	}
}

void Service::setBaseUrl(const QUrl& url)
{
	m_url = url;
	m_url.setPath("/onvif/device_service");
}

void Service::setCompleteUrl(const QUrl& url)
{
	m_url = url;
}

void Service::setUseWSUsernameToken(bool bUseWSUsernameToken)
{
	m_bUseWSUsernameToken = bUseWSUsernameToken;
}

void Service::setCustomDateTime(const QDateTime& customDateTime)
{
	m_customDateTime = customDateTime;
	m_bUseCustomDateTime = true;
}

void Service::setQueryExecutor(IQueryExecutor* pExecutor)
{
	if(m_pQueryExecutor) {
		delete m_pQueryExecutor;
		m_pQueryExecutor = NULL;
	}
	m_pQueryExecutor = pExecutor;

}

int Service::lastErrorCode() const
{
	return m_iLastErrorCode;
}

const QString& Service::lastError() const
{
	return m_szLastError;
}

QNetworkRequest Service::buildNetworkRequest() const
{
	QNetworkRequest request(m_url);
	return request;
}

QByteArray Service::buildSoapMessage(const QString& szSerializedObject, const QList<QString>& listNamespaceDeclaration) const
{
	QString szDatetime;
	if(m_bUseCustomDateTime){
		szDatetime = m_customDateTime.toString(Qt::ISODate);
	}else{
		szDatetime = QDateTime::currentDateTime().toString(Qt::ISODate);
	}
	QString szNonce = buildNonce();
	QByteArray szNonce64 = szNonce.toLatin1().toBase64();
	QByteArray digestbytes = szNonce.toLatin1();
	digestbytes.append(szDatetime.toLatin1());
	digestbytes.append(m_url.password().toLatin1());
	QString szDigestPassword = QString(QCryptographicHash::hash(digestbytes, QCryptographicHash::Sha1).toBase64());

	QString szNamespaceTmp;
	QList<QString> listNamespaceDeclarationCopy = listNamespaceDeclaration;
	QString szNamespaceDeclaration = " xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\"";
	while(!listNamespaceDeclarationCopy.empty()){
		szNamespaceTmp = listNamespaceDeclarationCopy.first();
		szNamespaceDeclaration += " " + szNamespaceTmp;
		listNamespaceDeclarationCopy.removeAll(szNamespaceTmp);
	}

	QByteArray bytes;
	bytes += "<s:Envelope" + szNamespaceDeclaration.toUtf8() + ">";
	bytes +=     "<s:Header>";
	if(m_bUseWSUsernameToken){
		if(!m_url.userName().isEmpty() && !m_url.password().isEmpty()){
			bytes +=         "<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">";
			bytes +=             "<UsernameToken>";
			bytes +=                 "<Username>" + m_url.userName().toUtf8() + "</Username>";
			bytes +=                 "<Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">" + szDigestPassword.toUtf8() +  "</Password>";
			bytes +=                 "<Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">" + szNonce64 +  "</Nonce>";
			bytes +=                 "<Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">" + szDatetime.toUtf8() +  "</Created>";
			bytes +=             "</UsernameToken>";
			bytes +=         "</Security>";
		}
	}
	bytes +=     "</s:Header>";
	bytes +=     "<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">";
	bytes +=		szSerializedObject.toUtf8();
	bytes +=     "</s:Body>";
	bytes += "</s:Envelope>";
	return bytes;
}

QString Service::buildNonce() const
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	const int randomStringLength = 20;

	QString randomString;
	for(int i=0; i<randomStringLength; ++i) {
#ifdef USE_QRANDOMGENERATOR
		QRandomGenerator rand;
		int iRand = (int)rand.generate();
#else
		int index = qrand() % possibleCharacters.length();
#endif
		QChar nextChar = possibleCharacters.at(index);
		randomString.append(nextChar);
	}
	return randomString;
}

}
