/*
 * XSBoolean.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSBOOLEAN_H_
#define XSBOOLEAN_H_

#include <QString>

class XSBoolean
{
public:
	XSBoolean();
	virtual ~XSBoolean();

	void setValue(bool bValue);
	bool getValue() const;

	QString serialize() const;

	void setNull();
	bool isNull() const;

	void operator=(const XSBoolean& other);
	void operator=(bool bOther);
	bool operator==(const XSBoolean& other) const;
	bool operator==(bool bOther) const;

private:
	short m_iValue;

};



#endif /* XSBOOLEAN_H_ */
