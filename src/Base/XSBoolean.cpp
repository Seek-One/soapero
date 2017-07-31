/*
 * XSBoolean.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSBoolean.h"

XSBoolean::XSBoolean()
{
	m_iValue = -1;
}

XSBoolean::~XSBoolean()
{

}

void XSBoolean::setValue(bool bValue)
{
	m_iValue = (bValue ? 1 : 0);
}

bool XSBoolean::getValue() const
{
	return (m_iValue == 1 ? true : false);
}

QString XSBoolean::serialize() const
{
	return (m_iValue == 1 ? "True" : "False");
}

void XSBoolean::setNull()
{
	m_iValue = -1;
}

bool XSBoolean::isNull() const
{
	return (m_iValue == -1);
}

void XSBoolean::operator=(const XSBoolean& other)
{
	m_iValue = other.m_iValue;
}

void XSBoolean::operator=(bool bOther)
{
	m_iValue = (bOther ? 1 : 0);
}

bool XSBoolean::operator==(const XSBoolean& other) const
{
	return m_iValue == other.m_iValue;
}

bool XSBoolean::operator==(bool bOther) const
{
	return m_iValue == (bOther ? 1 : 0);
}
