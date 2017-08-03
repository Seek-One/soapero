/*
 * Service.h
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QString>
#include <QUrl>

#include <QtNetwork/QNetworkRequest>

namespace Onvif {

class Service
{
public:
	Service();
	virtual ~Service();

	void setUrl(const QUrl& url);
	void setEndPoint(const QString& szEndPoint);

    int lastErrorCode() const;
    QString lastError() const;

protected:
	QNetworkRequest initNetworkRequest() const;

	QUrl m_url;

	int m_iLastErrorCode;
	QString m_szLastError;
};

}


#endif /* SERVICE_H_ */
