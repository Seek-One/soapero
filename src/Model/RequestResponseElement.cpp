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

void RequestResponseElement::setType(const TypeSharedPtr& pType)
{
	m_pType = pType;
}

TypeSharedPtr RequestResponseElement::getType() const
{
	return m_pType;
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
