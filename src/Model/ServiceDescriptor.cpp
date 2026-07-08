//
// Created by ebeuque on 08/07/2026.
//

#include "ServiceDescriptor.h"

ServiceDescriptor::ServiceDescriptor()
{
}

ServiceDescriptor::~ServiceDescriptor()
{
}

void ServiceDescriptor::setServiceName(const QString& szServiceName)
{
	m_szServiceName = szServiceName;
}

const QString& ServiceDescriptor::getServiceName() const
{
	return m_szServiceName;
}

void ServiceDescriptor::setURL(const QString& szURL)
{
	m_szURL = szURL;
}

const QString& ServiceDescriptor::getURL() const
{
	return m_szURL;
}

void ServiceDescriptor::setFileName(const QString& szFileName)
{
	m_szFileName = szFileName;
}

const QString& ServiceDescriptor::getFileName() const
{
	return m_szFileName;
}