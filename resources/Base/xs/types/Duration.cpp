/*
 * XSDuration.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "Duration.h"

namespace XS {

Duration::Duration()
{
	m_bIsNull = true;
}

Duration::Duration(const QString& szValue)
{
	m_szValue = szValue;
	m_bIsNull = false;
}

Duration::Duration(const Duration& other)
{
	m_szValue = other.m_szValue;
	m_bIsNull = other.m_bIsNull;
}

Duration::~Duration()
{

}

void Duration::setValue(const QString& szValue)
{
	m_bIsNull = false;
	m_szValue = szValue;
}

const QString& Duration::getValue() const
{
	return m_szValue;
}

Duration& Duration::operator= (const Duration& other)
{
	m_szValue = other.m_szValue;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool Duration::operator== (const Duration& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_szValue == other.m_szValue);
}

bool Duration::operator!= (const Duration& other) const
{
	return !(*this == other);
}

Duration& Duration::operator= (const char* szValue)
{
	setValue(szValue);
	return *this;
}

bool Duration::operator== (const char* szValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_szValue == szValue);
}

bool Duration::operator!= (const char* szValue) const
{
	return !(*this == szValue);
}

Duration& Duration::operator= (const QString& szValue)
{
	setValue(szValue);
	return *this;
}

bool Duration::operator== (const QString& szValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_szValue == szValue);
}

bool Duration::operator!= (const QString& szValue) const
{
	return !(*this == szValue);
}

QString Duration::serialize() const
{
	return m_szValue;
}

void Duration::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed());
}

void Duration::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed());
}

bool Duration::isNull() const
{
	return m_bIsNull;
}

}
