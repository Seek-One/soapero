/*
 * AnySimpleType.cpp
 *
 *  Created on: 11 juil. 2018
 *      Author: alavier
 */

#include "AnySimpleType.h"

namespace XS {

AnySimpleType::AnySimpleType()
	: String()
{

}

AnySimpleType::AnySimpleType(const QString& szValue)
	: String(szValue)
{

}

AnySimpleType::AnySimpleType(const AnySimpleType& other)
	: String(other)
{

}

AnySimpleType::~AnySimpleType()
{

}

AnySimpleType& AnySimpleType::operator= (const AnySimpleType& other)
{
	String::operator=(other);
	return *this;
}

AnySimpleType& AnySimpleType::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
