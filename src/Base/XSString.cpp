/*
 * XSString.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSString.h"

XSString::XSString()
{

}

XSString::~XSString()
{

}

void XSString::setValue(const QString& szValue)
{
	m_szValue = szValue;
}

QString XSString::getValue() const
{
	return m_szValue;
}

QString XSString::serialize() const
{
	return m_szValue;
}
