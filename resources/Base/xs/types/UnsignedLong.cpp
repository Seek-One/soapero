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
