/*
 * RequestResponseElement.cpp
 *
 *  Created on: 24 juil. 2017
 *      Author: lgruber
 */

#include "RequestResponseElement.h"

RequestResponseElement::RequestResponseElement()
{

}

RequestResponseElement::~RequestResponseElement()
{

}

RequestResponseElementSharedPtr RequestResponseElement::create()
{
	return RequestResponseElementSharedPtr(new RequestResponseElement());
}

void RequestResponseElement::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

void RequestResponseElement::setLocalName(const QString& szLocalName)
{
	m_szLocalName = szLocalName;
}

QString RequestResponseElement::getQualifedName() const
{
	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper() + m_szLocalName;
	}
}

QString RequestResponseElement::getNamespace() const
{
	return m_szNamespace;
}

QString RequestResponseElement::getLocalName() const
{
	return m_szLocalName;
}

void RequestResponseElement::setComplexType(const ComplexTypeSharedPtr& pComplexType)
{
	m_pComplexType = pComplexType;
}

ComplexTypeSharedPtr RequestResponseElement::getComplexType() const
{
	return m_pComplexType;
}

RequestResponseElementList::RequestResponseElementList()
	:QList<RequestResponseElementSharedPtr>()
{

}

RequestResponseElementList::~RequestResponseElementList()
{

}

RequestResponseElementListSharedPtr RequestResponseElementList::create()
{
	return RequestResponseElementListSharedPtr(new RequestResponseElementList());
}

