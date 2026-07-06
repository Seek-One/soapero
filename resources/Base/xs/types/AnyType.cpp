/*
 * XSAnyType.cpp
 *
 *  Created on: 5 juil. 2018
 *      Author: alavier
 */

#include "AnyType.h"

namespace XS {

AnyType::AnyType()
	: String()
{

}

AnyType::AnyType(const QString& szValue)
	: String(szValue)
{

}

AnyType::AnyType(const AnyType& other)
	: String(other)
{

}

AnyType::~AnyType()
{

}

AnyType& AnyType::operator= (const AnyType& other)
{
	String::operator=(other);
	return *this;
}

AnyType& AnyType::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
