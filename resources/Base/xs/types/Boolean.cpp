/*
 * XSBoolean.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "Boolean.h"

namespace XS {

Boolean::Boolean()
{
	m_bValue = false;
	m_bIsNull = true;
}

Boolean::Boolean(bool bValue)
{
	m_bValue = bValue;
	m_bIsNull = false;
}

Boolean::Boolean(const Boolean& other)
{
	m_bValue = other.m_bValue;
	m_bIsNull = other.m_bIsNull;
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

Boolean& Boolean::operator= (const Boolean& other)
{
	m_bIsNull = other.m_bIsNull;
	m_bValue = other.m_bValue;
	return (*this);
}

bool Boolean::operator== (const Boolean& other) const
{
	return (m_bIsNull == other.m_bIsNull) && (m_bValue == other.m_bValue);
}

bool Boolean::operator!= (const Boolean& other) const
{
	return (m_bIsNull != other.m_bIsNull) || (m_bValue != other.m_bValue);
}

Boolean& Boolean::operator= (bool bValue)
{
	m_bIsNull = false;
	m_bValue = bValue;
	return (*this);
}

bool Boolean::operator== (bool bValue) const
{
	return (m_bIsNull == false) && (m_bValue == bValue);
}

bool Boolean::operator!= (bool bValue) const
{
	return (m_bIsNull != false) || (m_bValue != bValue);
}

QString Boolean::serialize() const
{
	return (m_bValue ? "true" : "false");
}

void Boolean::deserialize(const QDomElement& element)
{
	if(element.text().trimmed() == "true") {
		setValue(true);
	}else if(element.text().trimmed() == "false"){
		setValue(false);
	}
}

void Boolean::deserialize(const QDomAttr& attr)
{
	if(attr.value().trimmed() == "true") {
		setValue(true);
	}else if(attr.value().trimmed() == "false"){
		setValue(false);
	}
}

bool Boolean::isNull() const
{
	return m_bIsNull;
}

}
