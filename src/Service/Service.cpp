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

void Service::setEndPoint(const QUrl& endPoint)
{

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
	return QNetworkRequest();
}

}
