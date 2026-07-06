/*
 * XSFloat.cpp
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#include "Float.h"

namespace XS {

Float::Float()
{
	m_fValue = 0.f;
	m_bIsNull = true;
}

Float::Float(float fValue)
{
	m_fValue = fValue;
	m_bIsNull = false;
}

Float::Float(const Float& other)
{
	m_fValue = other.m_fValue;
	m_bIsNull = other.m_bIsNull;
}

Float::~Float()
{

}

void Float::setValue(float fValue)
{
	m_bIsNull = false;
	m_fValue = fValue;
}

float Float::getValue() const
{
	return m_fValue;
}

Float& Float::operator= (const Float& other)
{
	m_fValue = other.m_fValue;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool Float::operator== (const Float& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_fValue == other.m_fValue);
}

bool Float::operator!= (const Float& other) const
{
	return !(*this == other);
}

Float& Float::operator= (float fValue)
{
	setValue(fValue);
	return *this;
}

bool Float::operator== (float fValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_fValue == fValue);
}

bool Float::operator!= (float fValue) const
{
	return !(*this == fValue);
}

QString Float::serialize() const
{
	return QString::number((double) m_fValue);
}

void Float::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed().toFloat());
}

void Float::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed().toFloat());
}

bool Float::isNull() const
{
	return m_bIsNull;
}

}
