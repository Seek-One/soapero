/*
 * XSInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "Integer.h"

namespace XS {

Integer::Integer()
{
	m_iValue = 0;
	m_bIsNull = true;
}

Integer::Integer(int iValue)
{
	m_iValue = iValue;
	m_bIsNull = false;
}

Integer::Integer(const Integer& other)
{
	m_iValue = other.m_iValue;
	m_bIsNull = other.m_bIsNull;
}

Integer::~Integer()
{

}

void Integer::setValue(int iValue)
{
	m_bIsNull = false;
	m_iValue = iValue;
}

int Integer::getValue() const
{
	return m_iValue;
}

Integer& Integer::operator= (const Integer& other)
{
	m_bIsNull = other.m_bIsNull;
	m_iValue = other.m_iValue;
	return (*this);
}

bool Integer::operator== (const Integer& other) const
{
	return (m_bIsNull == other.m_bIsNull) && (m_iValue == other.m_iValue);
}

bool Integer::operator!= (const Integer& other) const
{
	return (m_bIsNull != other.m_bIsNull) || (m_iValue != other.m_iValue);
}

Integer& Integer::operator= (int iValue)
{
	m_bIsNull = false;
	m_iValue = iValue;
	return (*this);
}

bool Integer::operator== (int iValue) const
{
	return (m_bIsNull == false) && (m_iValue == iValue);
}

bool Integer::operator!= (int iValue) const
{
	return (m_bIsNull != false) || (m_iValue != iValue);
}

QString Integer::serialize() const
{
	return QString::number(m_iValue);
}

void Integer::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed().toInt());
}

void Integer::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed().toInt());
}

bool Integer::isNull() const
{
	return m_bIsNull;
}

}
