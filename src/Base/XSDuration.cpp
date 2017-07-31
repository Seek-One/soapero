/*
 * XSDuration.cpp
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#include "XSDuration.h"

XSDuration::XSDuration()
{

}

XSDuration::~XSDuration()
{

}

void XSDuration::setDuration(const QString& szValue)
{
	m_szValue = szValue;
}

QString XSDuration::getValue() const
{
	return m_szValue;
}

QString XSDuration::serialize() const
{
	return m_szValue;
}
