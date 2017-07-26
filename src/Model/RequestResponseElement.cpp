/*
 * RequestResponseElement.cpp
 *
 *  Created on: 24 juil. 2017
 *      Author: lgruber
 */

#include "RequestResponseElement.h"

RequestResponseElement::RequestResponseElement()
	:Classname()
{

}

RequestResponseElement::~RequestResponseElement()
{

}

RequestResponseElementSharedPtr RequestResponseElement::create()
{
	return RequestResponseElementSharedPtr(new RequestResponseElement());
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

RequestResponseElementSharedPtr RequestResponseElementList::getByName(const QString& szLocalName, const QString& szNamespace)
{
	RequestResponseElementList::const_iterator element;
	for(element = constBegin(); element != constEnd(); ++element) {
		if( (*element)->getLocalName() == szLocalName &&
				(*element)->getNamespace() == szNamespace) {
			return *element;
		}
	}
	return RequestResponseElementSharedPtr();
}
