/*
 * XSUnsignedInteger.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSUNSIGNEDINTEGER_H_
#define XSUNSIGNEDINTEGER_H_

#include <QString>

namespace Onvif {
namespace XS {

class UnsignedInteger
{
public:
	UnsignedInteger();
	virtual ~UnsignedInteger();

	void setValue(unsigned int iValue);
	unsigned int getValue() const;

	QString serialize() const;

	bool isNull() const;

private:
	unsigned int m_iValue;
	bool m_bIsNull;

};

}
}

#endif /* XSUNSIGNEDINTEGER_H_ */
