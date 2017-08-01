/*
 * XSUnsignedInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSUnsignedInteger.h"

namespace XS {

UnsignedInteger::UnsignedInteger()
{
	m_iValue = -1;
}

UnsignedInteger::~UnsignedInteger()
{

}

void UnsignedInteger::setValue(unsigned int iValue)
{
	m_iValue = (int)iValue;
}

unsigned int UnsignedInteger::getValue() const
{
	return (unsigned int)m_iValue;
}

QString UnsignedInteger::serialize() const
{
	return QString::number(m_iValue);
}

}
