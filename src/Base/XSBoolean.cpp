/*
 * XSBoolean.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSBoolean.h"

namespace Onvif {
namespace XS {

Boolean::Boolean()
{
	m_iValue = -1;
}

Boolean::~Boolean()
{

}

void Boolean::setValue(bool bValue)
{
	m_iValue = (bValue ? 1 : 0);
}

bool Boolean::getValue() const
{
	return (m_iValue == 1 ? true : false);
}

QString Boolean::serialize() const
{
	return (m_iValue == 1 ? "True" : "False");
}

void Boolean::setNull()
{
	m_iValue = -1;
}

bool Boolean::isNull() const
{
	return (m_iValue == -1);
}

void Boolean::operator=(const XS::Boolean& other)
{
	m_iValue = other.m_iValue;
}

void Boolean::operator=(bool bOther)
{
	m_iValue = (bOther ? 1 : 0);
}

bool Boolean::operator==(const XS::Boolean& other) const
{
	return m_iValue == other.m_iValue;
}

bool Boolean::operator==(bool bOther) const
{
	return m_iValue == (bOther ? 1 : 0);
}

}
}
