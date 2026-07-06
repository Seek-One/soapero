/*
 * XSNonNegativeInteger.cpp
 *
 *  Created on: 10 juil. 2018
 *      Author: alavier
 */

#include "NonNegativeInteger.h"

namespace XS {

NonNegativeInteger::NonNegativeInteger()
	: UnsignedInteger()
{

}

NonNegativeInteger::NonNegativeInteger(unsigned int iValue)
	: UnsignedInteger(iValue)
{

}

NonNegativeInteger::NonNegativeInteger(const NonNegativeInteger& other)
	: UnsignedInteger(other)
{

}

NonNegativeInteger::~NonNegativeInteger()
{

}

NonNegativeInteger& NonNegativeInteger::operator= (const NonNegativeInteger& other)
{
	UnsignedInteger::operator=(other);
	return *this;
}

NonNegativeInteger& NonNegativeInteger::operator= (unsigned int iValue)
{
	UnsignedInteger::operator=(iValue);
	return *this;
}

}
