/*
 * Operation.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include <Utils/ModelUtils.h>

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
	if(m_pSoapEnvFaultType){
		szDeclaration += ", ";
		szDeclaration += m_pSoapEnvFaultType->getNameWithNamespace();
		szDeclaration += "& ";
		szDeclaration += m_pSoapEnvFaultType->getLocalName();
	}
	szDeclaration += ");";

	return szDeclaration;
}

QString Operation::getOperationDefinition(const QString& szClassname, const QString& szNamespace) const
{
	QString szInputName = ModelUtils::getUncapitalizedName(m_pInputMessage->getParameter()->getLocalName());
	QString szOutputName = ModelUtils::getUncapitalizedName(m_pOutputMessage->getParameter()->getLocalName());

	QString szDefinition;
	szDefinition += "bool " + szClassname + "::" + m_szName + "(const " + m_pInputMessage->getParameter()->getNameWithNamespace() +
			"& " + szInputName + ", " + m_pOutputMessage->getParameter()->getNameWithNamespace() +
			"& " + szOutputName;

	if(m_pSoapEnvFaultType){
		szDefinition += ", " + m_pSoapEnvFaultType->getNameWithNamespace() + "& " + m_pSoapEnvFaultType->getLocalName() + ")" CRLF;
	}else{
		szDefinition += ")" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\tbool bGoOn = true;" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tQNetworkRequest request = buildNetworkRequest();" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"Content-Type\").toLatin1(), QString(\"application/soap+xml; charset=utf-8; action=\\\"" + m_szSoapAction + "\\\"\").toLatin1());" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"Accept-Encoding\").toLatin1(), QString(\"gzip, deflate\").toLatin1());" CRLF;
	szDefinition += "\trequest.setRawHeader(QString(\"SoapAction\").toLatin1(), QString(\"" + m_szSoapAction + "\").toLatin1());" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tQByteArray soapMessage = buildSoapMessage(" + szInputName + ".serialize(), " + m_pInputMessage->getParameter()->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;

	// Debug request
	szDefinition += "\tif(m_bDebug){" CRLF;
	szDefinition += "\t\tqWarning(\"Message:\\n%s\", qPrintable(QString::fromUtf8(soapMessage)));" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += CRLF;
	szDefinition += "\tIQueryExecutorResponse response = m_pQueryExecutor->execQuery(request, soapMessage);" CRLF;
	szDefinition += "\tQString szErrorMsg;" CRLF;
	szDefinition += "\tint iErrorLine = -1;" CRLF;
	szDefinition += "\tint iErrorColumn = -1;" CRLF;
	szDefinition += "\tQDomDocument doc;" CRLF;
	szDefinition += "\tQMap<QString, QString> namespaceRoutingMap;" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tif(doc.setContent(response.getResponse(), &szErrorMsg, &iErrorLine, &iErrorColumn)){" CRLF;
	szDefinition += "\t\tnamespaceRoutingMap = buildNamespaceRoutingMap(doc);" CRLF;
	szDefinition += CRLF;
	if(m_pSoapEnvFaultType){
		szDefinition += "\t\tQString szNamespace = namespaceRoutingMap.value(SOAP_ENV_URI, " + szNamespace + "::" + m_pSoapEnvFaultType->getNameWithNamespace() + "TargetNamespace);" CRLF;
		szDefinition += "\t\tQString szFaultTagName = szNamespace + \":Fault\";" CRLF;
		szDefinition += "\t\tif(doc.elementsByTagName(szFaultTagName).size() > 0){" CRLF;
		szDefinition += "\t\t\tQDomElement root = doc.elementsByTagName(szFaultTagName).at(0).toElement();" CRLF;
		szDefinition += "\t\t\tFault.deserialize(root);" CRLF;
		szDefinition += "\t\t\tbGoOn = false;" CRLF;
		szDefinition += "\t\t}else{" CRLF;
	}
	szDefinition += "\t\t" + QString(m_pSoapEnvFaultType ? "\t" : "") + "QDomElement root = doc.elementsByTagName(namespaceRoutingMap.value(SOAP_ENV_URI, DEFAULT_SOAP_ENV_NAMESPACE) + \":Body\").at(0).firstChildElement();" CRLF;
	szDefinition += "\t\t" + QString(m_pSoapEnvFaultType ? "\t" : "") + szOutputName + ".deserialize(root);" CRLF;
	if(m_pSoapEnvFaultType){
		szDefinition += "\t\t}" CRLF;
	}
	szDefinition += "\t}else{" CRLF;
	szDefinition += "\t\tbGoOn = false;" CRLF;
	szDefinition += "\t\tqWarning(\"[" + szNamespace + "::" + m_szName + "] Error during parsing response : %s (%d:%d)\", qPrintable(szErrorMsg), iErrorLine, iErrorColumn);" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;
	szDefinition += "\tif(response.getHttpStatusCode() != 200){" CRLF;
	szDefinition += "\t\tbGoOn = false;" CRLF;
	szDefinition += "\t\tqWarning(\"[" + szNamespace + "::" + m_szName + "] Error with HTTP status code: %d\", response.getHttpStatusCode());" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += "\tif(m_bDebug){" CRLF;
	szDefinition += "\t\tqWarning(\"Response:\\n%s\", qPrintable(QString::fromUtf8(response.getResponse())));" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += CRLF;

	szDefinition += "\treturn bGoOn;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition;
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
