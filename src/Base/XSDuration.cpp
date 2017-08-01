/*
 * XSDuration.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSDuration.h"

namespace XS {

Duration::Duration()
{

}

Duration::~Duration()
{

}

void Duration::setDuration(const QString& szValue)
{
	m_szValue = szValue;
}

QString Duration::getValue() const
{
	return m_szValue;
}

QString Duration::serialize() const
{
	return m_szValue;
}

}
