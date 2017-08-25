/*
 * XSDuration.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSDuration.h"

namespace Onvif {
namespace XS {

Duration::Duration()
{
	m_bIsNull = true;
}

Duration::~Duration()
{

}

void Duration::setValue(const QString& szValue)
{
	m_bIsNull = false;
	m_szValue = szValue;
}

const QString& Duration::getValue() const
{
	return m_szValue;
}

QString Duration::serialize() const
{
	return m_szValue;
}

void Duration::deserialize(const QDomElement& element)
{
	setValue(element.text().trimmed());
}

void Duration::deserialize(const QDomAttr& attr)
{
	setValue(attr.value().trimmed());
}

bool Duration::isNull() const
{
	return m_bIsNull;
}

}
}
