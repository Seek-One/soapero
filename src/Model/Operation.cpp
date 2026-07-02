/*
 * Operation.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include <Utils/ModelUtils.h>

#include "Operation.h"

#include "../common.h"

Operation::Operation()
{

}

Operation::~Operation()
{

}

OperationSharedPtr Operation::create()
{
	return OperationSharedPtr(new Operation());
}

void Operation::setName(const QString& szName)
{
	m_szName = szName;
}

QString Operation::getName() const
{
	return m_szName;
}

void Operation::setInputMessage(const MessageSharedPtr& pMessage)
{
	m_pInputMessage = pMessage;
}

MessageSharedPtr Operation::getInputMessage() const
{
	return m_pInputMessage;
}

void Operation::setOutputMessage(const MessageSharedPtr& pMessage)
{
	m_pOutputMessage = pMessage;
}

MessageSharedPtr Operation::getOutputMessage() const
{
	return m_pOutputMessage;
}

void Operation::setSoapEnvelopeFaultType(const ComplexTypeSharedPtr& pSoapEnvFaultType)
{
	m_pSoapEnvFaultType = pSoapEnvFaultType;
}

const ComplexTypeSharedPtr& Operation::getSoapEnvelopeFaultType() const
{
	return m_pSoapEnvFaultType;
}

void Operation::setSoapAction(const QString& szSoapAction)
{
	m_szSoapAction = szSoapAction;
}

QString Operation::getSoapAction() const
{
	return m_szSoapAction;
}


OperationList::OperationList()
	:QList<OperationSharedPtr>()
{

}

OperationList::~OperationList()
{

}

OperationListSharedPtr OperationList::create()
{
	return OperationListSharedPtr(new OperationList());
}

OperationSharedPtr OperationList::getByName(const QString& szName)
{
	OperationList::const_iterator operation;
	for(operation = constBegin(); operation != constEnd(); ++operation) {
		if( (*operation)->getName() == szName) {
			return *operation;
		}
	}
	return OperationSharedPtr();
}
