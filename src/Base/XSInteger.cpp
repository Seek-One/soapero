/*
 * XSInteger.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSInteger.h"

namespace XS {

Integer::Integer()
{
	m_iValue = 0;
	m_bIsNull = true;
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
