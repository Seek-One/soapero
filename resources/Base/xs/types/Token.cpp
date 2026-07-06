/*
 * XSToken.cpp
 *
 *  Created on: 26 juin 2018
 *      Author: alavier
 */

#include "Token.h"

namespace XS {

Token::Token()
	: String()
{

}

Token& Token::operator= (const char* szValue)
{
	String::operator=(szValue);
	return *this;
}

Token::Token(const QString& szValue)
	: String(szValue)
{

}

Token::Token(const Token& other)
	: String(other)
{

}

Token::~Token()
{

}

Token& Token::operator= (const Token& other)
{
	String::operator=(other);
	return *this;
}

Token& Token::operator= (const QString& szValue)
{
	String::operator=(szValue);
	return *this;
}

}
