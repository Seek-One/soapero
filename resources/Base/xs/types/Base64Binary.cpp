/*
 * XSBase64Binary.cpp
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#include "Base64Binary.h"

namespace XS {

Base64Binary::Base64Binary()
	: String()
{

}

Base64Binary& Base64Binary::operator= (const char* szValue)
{
	String::operator=(szValue);
	return *this;
}

Base64Binary::Base64Binary(const QString& szValue)
	: String(szValue)
{

}

Base64Binary::Base64Binary(const Base64Binary& other)
	: String(other)
{

}

Base64Binary::~Base64Binary()
{

}

Base64Binary& Base64Binary::operator= (const Base64Binary& other)
{
	String::operator=(other);
	return *this;
}

Base64Binary& Base64Binary::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
