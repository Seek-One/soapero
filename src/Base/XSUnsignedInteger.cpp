/*
 * XSUnsignedInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSUnsignedInteger.h"

XSUnsignedInteger::XSUnsignedInteger()
{
	m_iValue = -1;
}

XSUnsignedInteger::~XSUnsignedInteger()
{

}

void XSUnsignedInteger::setValue(unsigned int iValue)
{
	m_iValue = (int)iValue;
}

unsigned int XSUnsignedInteger::getValue() const
{
	return (unsigned int)m_iValue;
}

QString XSUnsignedInteger::serialize() const
{
	return QString::number(m_iValue);
}
