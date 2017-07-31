/*
 * XSInteger.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSINTEGER_H_
#define XSINTEGER_H_

#include <QString>

class XSInteger
{
public:
	XSInteger();
	virtual ~XSInteger();

	void setValue(int iValue);
	int getValue() const;

	QString serialize() const;

private:
	int m_iValue;

};



#endif /* XSINTEGER_H_ */
