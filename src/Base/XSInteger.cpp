/*
 * XSInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSInteger.h"

XSInteger::XSInteger()
{
	m_iValue = 0;
}

XSInteger::~XSInteger()
{

}

void XSInteger::setValue(int iValue)
{
	m_iValue = iValue;
}

int XSInteger::getValue() const
{
	return m_iValue;
}

QString XSInteger::serialize() const
{
	return QString::number(m_iValue);
}

