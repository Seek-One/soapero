/*
 * Service.cpp
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

/* This will define the timeval struct */
#ifdef _MSC_VER
#include <winsock.h>
#else
#include <sys/time.h>
#endif

#include <QCryptographicHash>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "Service.h"

namespace SOAPERO {

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

#ifdef _MSC_VER
	static int p_gettimeofday(struct timeval* p, void* tz)
	{
		ULARGE_INTEGER ul; // As specified on MSDN.
		FILETIME ft;

		// Returns a 64-bit value representing the number of
		// 100-nanosecond intervals since January 1, 1601 (UTC).
		GetSystemTimeAsFileTime(&ft);

		// Fill ULARGE_INTEGER low and high parts.
		ul.LowPart = ft.dwLowDateTime;
		ul.HighPart = ft.dwHighDateTime;
		// Convert to microseconds.
		ul.QuadPart /= 10ULL;
		// Remove Windows to UNIX Epoch delta.
		ul.QuadPart -= 11644473600000000ULL;
		// Modulo to retrieve the microseconds.
		p->tv_usec = (long) (ul.QuadPart % 1000000LL);
		// Divide to retrieve the seconds.
		p->tv_sec = (long) (ul.QuadPart / 1000000LL);
		return 0;
	}
#else
#define p_gettimeofday gettimeofday
#endif

Service::Service()
{
	m_iLastErrorCode = -1;
	m_pQueryExecutor = new CustomQueryExecutor();
	m_bUseWSUsernameToken = false;
	m_bUseCustomDateTime = false;
#ifdef USE_QRANDOMGENERATOR
	// Initialize random number based on time with nano second precision.
	// This be be sure to don't have
	struct timeval tvNow;
	p_gettimeofday(&tvNow, NULL);
	quint64 iSec = (quint64)tvNow.tv_sec;
	quint64 iUSec = (quint64)tvNow.tv_usec;
	quint32 listSeedValue[4];
	listSeedValue[0] = (iSec >> 32);
	listSeedValue[1] = (iSec & 0xFFFFFFFF);
	listSeedValue[2] = (iUSec >> 32);
	listSeedValue[3] = (iUSec & 0xFFFFFFFF);
	m_rand = QRandomGenerator(listSeedValue, 4);
#else
	uint iSeed = (uint)(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF);
	qsrand(iSeed);
#endif
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
	QString szPossibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	int iRandomStringLength = 20;

	QString szRandomString;
	for(int i=0; i<iRandomStringLength; ++i) {
#ifdef USE_QRANDOMGENERATOR
		int index = (int)(m_rand.generate() % szPossibleCharacters.length());
#else
		int index = qrand() % szPossibleCharacters.length();
#endif
		QChar nextChar = szPossibleCharacters.at(index);
		szRandomString.append(nextChar);
	}
	return szRandomString;
}

}
