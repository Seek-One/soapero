/*
 * XSAnyURI.cpp
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#include "AnyURI.h"

namespace XS {

AnyURI::AnyURI()
	: String()
{

}

AnyURI& AnyURI::operator= (const char* szValue)
{
	String::operator=(szValue);
	return *this;
}

AnyURI::AnyURI(const QString& szValue)
	: String(szValue)
{

}

AnyURI::AnyURI(const AnyURI& other)
	: String(other)
{

}

AnyURI::~AnyURI()
{

}

AnyURI& AnyURI::operator= (const AnyURI& other)
{
	String::operator=(other);
	return *this;
}

AnyURI& AnyURI::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
