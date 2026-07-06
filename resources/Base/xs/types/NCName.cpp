/*
 * XSNCName.cpp
 *
 *  Created on: 22 juin 2018
 *      Author: alavier
 */

#include "NCName.h"

namespace XS {

NCName::NCName()
	: String()
{

}

NCName::NCName(const QString& szValue)
	: String(szValue)
{

}

NCName::NCName(const NCName& other)
	: String(other)
{

}

NCName::~NCName()
{

}

NCName& NCName::operator= (const NCName& other)
{
	String::operator=(other);
	return *this;
}

NCName& NCName::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
