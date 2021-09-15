//
// Created by ebeuque on 15/09/2021.
//

#include "QWSDLData.h"

QWSDLData::QWSDLData()
{

}

QWSDLData::~QWSDLData()
{

}

void QWSDLData::addLoadedURI(const QString& szURI)
{
	m_listLoadedURI.append(szURI);
}

bool QWSDLData::hasLoadedURI(const QString& szURI) const
{
	return m_listLoadedURI.contains(szURI);
}