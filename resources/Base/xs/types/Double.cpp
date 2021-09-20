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
	m_fValue = 0.f;
	m_bIsNull = true;
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
