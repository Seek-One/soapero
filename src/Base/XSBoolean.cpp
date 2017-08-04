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
	m_bValue = false;
	m_bIsNull = true;
}

Boolean::~Boolean()
{

}

void Boolean::setValue(bool bValue)
{
	m_bIsNull = false;
	m_bValue = bValue;
}

bool Boolean::getValue() const
{
	return m_bValue;
}

QString Boolean::serialize() const
{
	return (m_bValue ? "True" : "False");
}

bool Boolean::isNull() const
{
	return m_bIsNull;
}

}
}
