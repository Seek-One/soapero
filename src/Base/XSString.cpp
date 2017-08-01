/*
 * XSString.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSString.h"

namespace XS {

String::String()
{

}

String::~String()
{

}

void String::setValue(const QString& szValue)
{
	m_szValue = szValue;
}

QString String::getValue() const
{
	return m_szValue;
}

QString String::serialize() const
{
	return m_szValue;
}

}
