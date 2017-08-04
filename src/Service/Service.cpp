/*
 * Service.cpp
 *
 *  Created on: 3 août 2017
 *      Author: lgruber
 */

#include "Service.h"

namespace Onvif {

Service::Service()
{
	m_iLastErrorCode = -1;
}

Service::~Service()
{

}

void Service::setUrl(const QUrl& url)
{
	m_url = url;
	m_url.setPath("/onvif/device_service");
}

int Service::lastErrorCode() const
{
	return m_iLastErrorCode;
}

QString Service::lastError() const
{
	return m_szLastError;
}

QNetworkRequest Service::initNetworkRequest() const
{
	QNetworkRequest request;

	request.setUrl(m_url);
	return QNetworkRequest();
}

}
