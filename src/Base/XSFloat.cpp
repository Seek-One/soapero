/*
 * XSFloat.cpp
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#include "XSFloat.h"

namespace XS {

Float::Float()
{
	m_fValue = 0.f;
	m_bIsNull = true;
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
