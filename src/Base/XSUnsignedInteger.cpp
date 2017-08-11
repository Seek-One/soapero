/*
 * XSUnsignedInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSUnsignedInteger.h"

namespace Onvif {
namespace XS {

UnsignedInteger::UnsignedInteger()
{
	m_iValue = 0;
	m_bIsNull = true;
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
}
