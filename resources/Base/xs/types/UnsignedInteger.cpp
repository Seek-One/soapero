/*
 * XSUnsignedInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "UnsignedInteger.h"

namespace XS {

UnsignedInteger::UnsignedInteger()
{
	m_iValue = 0;
	m_bIsNull = true;
}

UnsignedInteger::UnsignedInteger(unsigned int iValue)
{
	m_iValue = iValue;
	m_bIsNull = false;
}

UnsignedInteger::UnsignedInteger(const UnsignedInteger& other)
{
	m_iValue = other.m_iValue;
	m_bIsNull = other.m_bIsNull;
}

UnsignedInteger::~UnsignedInteger()
{

}

void UnsignedInteger::setValue(unsigned int iValue)
{
	m_bIsNull = false;
	m_iValue = iValue;
}

unsigned int UnsignedInteger::getValue() const
{
	return m_iValue;
}

UnsignedInteger& UnsignedInteger::operator= (const UnsignedInteger& other)
{
	m_iValue = other.m_iValue;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool UnsignedInteger::operator== (const UnsignedInteger& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_iValue == other.m_iValue);
}

bool UnsignedInteger::operator!= (const UnsignedInteger& other) const
{
	return !(*this == other);
}

UnsignedInteger& UnsignedInteger::operator= (unsigned int iValue)
{
	setValue(iValue);
	return *this;
}

bool UnsignedInteger::operator== (unsigned int iValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_iValue == iValue);
}

bool UnsignedInteger::operator!= (unsigned int iValue) const
{
	return !(*this == iValue);
}

QString UnsignedInteger::serialize() const
{
	return QString::number(m_iValue);
}

void UnsignedInteger::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed().toUInt());
}

void UnsignedInteger::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed().toUInt());
}

bool UnsignedInteger::isNull() const
{
	return m_bIsNull;
}

}
