/*
 * XSDateTime.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: alavier
 */

#include "DateTime.h"

namespace XS {

DateTime::DateTime()
{
	m_bIsNull = true;
}

DateTime::~DateTime()
{

}

void DateTime::setValue(const QDateTime& dateTime)
{
	m_bIsNull = false;
	m_dateTime = dateTime;
}

const QDateTime& DateTime::getValue() const
{
	return m_dateTime;
}

QString DateTime::serialize() const
{
	return m_dateTime.toString("yyyy-MM-ddThh:mm:ss");
}

void DateTime::deserialize(const QDomElement& element)
{
	setValue(QDateTime::fromString(element.text(), Qt::ISODate));
}

void DateTime::deserialize(const QDomAttr& attr)
{
	setValue(QDateTime::fromString(attr.value(), Qt::ISODate));
}

bool DateTime::isNull() const
{
	return m_bIsNull;
}

}
