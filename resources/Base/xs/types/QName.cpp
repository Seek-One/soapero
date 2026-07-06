/*
 * XSQName.cpp
 *
 *  Created on: 21 juin 2018
 *      Author: alavier
 */

#include "QName.h"

namespace XS {

QName::QName()
	: String()
{

}

QName& QName::operator= (const char* szValue)
{
	String::operator=(szValue);
	return *this;
}

QName::QName(const QString& szValue)
	: String(szValue)
{

}

QName::QName(const QName& other)
	: String(other)
{

}

QName::~QName()
{

}

QName& QName::operator= (const QName& other)
{
	String::operator=(other);
	return *this;
}

QName& QName::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
