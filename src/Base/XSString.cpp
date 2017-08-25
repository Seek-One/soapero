/*
 * XSString.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSString.h"

namespace XS {

String::String()
{

}

String::~String()
{

}

void String::setValue(const QString& szValue)
{
	m_szValue = szValue;
}

const QString& String::getValue() const
{
	return m_szValue;
}

QString String::serialize() const
{
	return m_szValue;
}

void String::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed());
}

void String::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed());
}

bool String::isNull() const
{
	return m_szValue.isNull();
}

}
