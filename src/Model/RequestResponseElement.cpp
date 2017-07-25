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

void RequestResponseElement::setName(const QString& szName)
{
	m_szName = szName;
}

QString RequestResponseElement::getName() const
{
	return m_szName;
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

