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

void Operation::setSoapAction(const QString& szSoapAction)
{
	m_szSoapAction = szSoapAction;
}

QString Operation::getSoapAction() const
{
	return m_szSoapAction;
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
	"\tQNetworkRequest request = buildNetworkRequest();" CRLF
	"\trequest.setRawHeader(QString(\"Content-Type\").toLatin1(), QString(\"application/soap+xml; charset=utf-8; action=\\\"%6\\\"\").toLatin1());" CRLF
	"\trequest.setRawHeader(QString(\"Accept-Encoding\").toLatin1(), QString(\"gzip, deflate\").toLatin1());" CRLF
	"\trequest.setRawHeader(QString(\"SoapAction\").toLatin1(), QString(\"%6\").toLatin1());" CRLF
	CRLF
	"\tQByteArray soapMessage = buildSoapMessage(%3.serialize());" CRLF
	CRLF
	"\tQNetworkReply *reply = manager->post(request, soapMessage);" CRLF
	"\tif(reply->waitForReadyRead(TIMEOUT_MSEC)) {" CRLF
		"\t\tQByteArray bytes = reply->readAll();" CRLF
		"\t\tQString szErrorMsg;" CRLF
		"\t\tint iErrorLine = -1;" CRLF
		"\t\tint iErrorColumn = -1;" CRLF
		"\t\tQDomDocument doc;" CRLF CRLF
		"\t\tif(doc.setContent(bytes, &szErrorMsg, &iErrorLine, &iErrorColumn)) {" CRLF
		"\t\t\tQDomElement root=doc.elementsByTagName(\"Soap::body\").at(0).firstChildElement();" CRLF
		"\t\t\t%5.deserialize(root);" CRLF
		"\t\t} else {" CRLF
		"\t\t\tqWarning(\"[%0::%1] Error during parsing response : %s (%d:%d)\", qPrintable(szErrorMsg), iErrorLine, iErrorColumn);" CRLF
		"\t\t}" CRLF
	"\t}" CRLF
	"\treturn false;" CRLF
	"}" CRLF;

	return szDefinition.arg(szClassname)
			.arg(m_szName)
			.arg(m_pInputMessage->getParameter()->getNameWithNamespace())
			.arg(m_pInputMessage->getParameter()->getLocalName())
			.arg(m_pOutputMessage->getParameter()->getNameWithNamespace())
			.arg(m_pOutputMessage->getParameter()->getLocalName())
			.arg(m_szSoapAction);
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
