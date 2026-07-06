/*
 * XSDouble.cpp
 *
 *  Created on: 11 juil. 2018
 *      Author: alavier
 */

#include "Double.h"

namespace XS {

Double::Double()
{
	m_fValue = 0.;
	m_bIsNull = true;
}

Double::Double(double fValue)
{
	m_fValue = fValue;
	m_bIsNull = false;
}

Double::Double(const Double& other)
{
	m_fValue = other.m_fValue;
	m_bIsNull = other.m_bIsNull;
}

Double::~Double()
{

}

void Double::setValue(double fValue)
{
	m_bIsNull = false;
	m_fValue = fValue;
}

double Double::getValue() const
{
	return m_fValue;
}

Double& Double::operator= (const Double& other)
{
	m_fValue = other.m_fValue;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool Double::operator== (const Double& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_fValue == other.m_fValue);
}

bool Double::operator!= (const Double& other) const
{
	return !(*this == other);
}

Double& Double::operator= (double fValue)
{
	setValue(fValue);
	return *this;
}

bool Double::operator== (double fValue) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_fValue == fValue);
}

bool Double::operator!= (double fValue) const
{
	return !(*this == fValue);
}

QString Double::serialize() const
{
	return QString::number(m_fValue);
}

void Double::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed().toDouble());
}

void Double::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed().toDouble());
}

bool Double::isNull() const
{
	return m_bIsNull;
}

}
