/*
 * Service.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include "Service.h"

Service::Service()
{
	m_pOperationList = OperationList::create();
}

Service::~Service()
{

}

ServiceSharedPtr Service::create()
{
	return ServiceSharedPtr(new Service());
}

QString Service::getName() const
{
	return m_szName;
}

void Service::setName(const QString& szName)
{
	m_szName = szName;
}

QString Service::getTargetNamespace() const
{
	return m_szTargetNamespace;
}

void Service::setTargetNamespace(const QString& szTargetNamespace)
{
	m_szTargetNamespace = szTargetNamespace;
}

OperationListSharedPtr Service::getOperationList() const
{
	return m_pOperationList;
}

void Service::addOperation(const OperationSharedPtr& pOperation)
{
	m_pOperationList->append(pOperation);
}


