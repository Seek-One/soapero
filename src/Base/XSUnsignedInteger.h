/*
 * XSUnsignedInteger.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSUNSIGNEDINTEGER_H_
#define XSUNSIGNEDINTEGER_H_

#include <QString>

namespace XS {

class UnsignedInteger
{
public:
	UnsignedInteger();
	virtual ~UnsignedInteger();

	void setValue(unsigned int iValue);
	unsigned int getValue() const;

	QString serialize() const;

private:
	int m_iValue;

};

}

#endif /* XSUNSIGNEDINTEGER_H_ */
