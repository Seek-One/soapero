/*
 * XSUnsignedLong.cpp
 *
 *  Created on: 2 juil. 2018
 *      Author: alavier
 */

#include "UnsignedLong.h"

namespace XS {

UnsignedLong::UnsignedLong()
{
	m_iValue = 0;
	m_bIsNull = true;
}

UnsignedLong::UnsignedLong(unsigned long iValue)
{
	m_iValue = iValue;
	m_bIsNull = false;
}

UnsignedLong::UnsignedLong(const UnsignedLong& other)
{
	m_iValue = other.m_iValue;
	m_bIsNull = other.m_bIsNull;
}

UnsignedLong::~UnsignedLong()
{

}

void UnsignedLong::setValue(unsigned long iValue)
{
	m_bIsNull = false;
	m_iValue = iValue;
}

unsigned long UnsignedLong::getValue() const
{
	return m_iValue;
}

UnsignedLong& UnsignedLong::operator= (const UnsignedLong& other)
{
	m_iValue = other.m_iValue;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool UnsignedLong::operator== (const UnsignedLong& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_iValue == other.m_iValue);
}

bool UnsignedLong::operator!= (const UnsignedLong& other) const
{
	return !(*this == other);
}

UnsignedLong& UnsignedLong::operator= (unsigned long iValue)
{
	setValue(iValue);
	return *this;
}

bool UnsignedLong::operator== (unsigned long iValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_iValue == iValue);
}

bool UnsignedLong::operator!= (unsigned long iValue) const
{
	return !(*this == iValue);
}

QString UnsignedLong::serialize() const
{
	return QString::number(m_iValue);
}

void UnsignedLong::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed().toUInt());
}

void UnsignedLong::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed().toULong());
}

bool UnsignedLong::isNull() const
{
	return m_bIsNull;
}

}
