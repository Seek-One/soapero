/*
 * Operation.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include "Operation.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif


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

QString Operation::getOperationDeclaration() const
{
	QString szDeclaration;
	szDeclaration += "bool ";
	szDeclaration += m_szName;
	szDeclaration += "(const ";
	szDeclaration += m_pInputMessage->getParameter()->getNameWithNamespace();
	szDeclaration += "& ";
	szDeclaration += m_pInputMessage->getParameter()->getLocalName();
	szDeclaration += ", ";
	szDeclaration += m_pOutputMessage->getParameter()->getNameWithNamespace();
	szDeclaration += "& ";
	szDeclaration += m_pOutputMessage->getParameter()->getLocalName();
	szDeclaration += ");";

	return szDeclaration;
}

QString Operation::getOperationDefinition(const QString& szClassname) const
{
	QString szDefinition = ""
	"bool %0::%1(const %2& %3, %4& %5)" CRLF
	"{" CRLF
	"\tQNetworkAccessManager *manager = new QNetworkAccessManager();" CRLF
	CRLF
	"\tQNetworkRequest request = initNetworkRequest();" CRLF
	CRLF
	"\tQNetworkReply *reply = manager->post(request, %3.serialize().toUtf8());" CRLF
	"\tif(reply->waitForReadyRead(TIMEOUT_MSEC)) {" CRLF
		"\t\tQByteArray bytes = reply->readAll();" CRLF
		"\t\tQString szValue(bytes);" CRLF
		"\t\t//GetServiceCapabilitiesResponse.deserialize(szValue);" CRLF
	"\t}" CRLF
	"\treturn false;" CRLF
	"}" CRLF;

	return szDefinition.arg(szClassname)
			.arg(m_szName)
			.arg(m_pInputMessage->getParameter()->getNameWithNamespace())
			.arg(m_pInputMessage->getParameter()->getLocalName())
			.arg(m_pOutputMessage->getParameter()->getNameWithNamespace())
			.arg(m_pOutputMessage->getParameter()->getLocalName());
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
