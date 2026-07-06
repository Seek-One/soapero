/*
 * XSHexBinary.cpp
 *
 *  Created on: 10 juil. 2018
 *      Author: alavier
 */

#include "HexBinary.h"

namespace XS {

HexBinary::HexBinary()
	: String()
{

}

HexBinary::HexBinary(const QString& szValue)
	: String(szValue)
{

}

HexBinary::HexBinary(const HexBinary& other)
	: String(other)
{

}

HexBinary::~HexBinary()
{

}

HexBinary& HexBinary::operator= (const HexBinary& other)
{
	String::operator=(other);
	return *this;
}

HexBinary& HexBinary::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
