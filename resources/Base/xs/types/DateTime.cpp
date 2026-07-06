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

DateTime::DateTime(const QDateTime& value)
{
	m_dateTime = value;
	m_bIsNull = false;
}

DateTime::DateTime(const DateTime& other)
{
	m_dateTime = other.m_dateTime;
	m_bIsNull = other.m_bIsNull;
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

DateTime& DateTime::operator= (const DateTime& other)
{
	m_dateTime = other.m_dateTime;
	m_bIsNull = other.m_bIsNull;
	return *this;
}

bool DateTime::operator== (const DateTime& other) const
{
	if (m_bIsNull && other.m_bIsNull) {
		return true;
	}
	if (m_bIsNull || other.m_bIsNull) {
		return false;
	}
	return (m_dateTime == other.m_dateTime);
}

bool DateTime::operator!= (const DateTime& other) const
{
	return !(*this == other);
}

DateTime& DateTime::operator= (const QDateTime& value)
{
	setValue(value);
	return *this;
}

bool DateTime::operator== (const QDateTime& value) const
{
	if (m_bIsNull) {
		return false;
	}
	return (m_dateTime == value);
}

bool DateTime::operator!= (const QDateTime& value) const
{
	return !(*this == value);
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
