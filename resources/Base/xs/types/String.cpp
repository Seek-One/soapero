/*
 * XSString.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "String.h"

namespace XS {

String::String()
{

}

String::String(const char* szValue)
{
	m_szValue = szValue;
}

String::String(const QString& szValue)
{
	m_szValue = szValue;
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

String& String::operator= (const String& other)
{
	m_szValue = other.m_szValue;
	return (*this);
}

bool String::operator== (const String& other) const
{
	return (m_szValue == other.m_szValue);
}

bool String::operator!= (const String& other) const
{
	return (m_szValue != other.m_szValue);
}

String& String::operator= (const char* szValue)
{
	m_szValue = szValue;
	return (*this);
}

bool String::operator== (const char* szValue) const
{
	return (m_szValue == szValue);
}

bool String::operator!= (const char* szValue) const
{
	return (m_szValue != szValue);
}

String& String::operator= (const QString& szValue)
{
	m_szValue = szValue;
	return (*this);
}

bool String::operator== (const QString& szValue) const
{
	return (m_szValue == szValue);
}

bool String::operator!= (const QString& szValue) const
{
	return (m_szValue != szValue);
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
