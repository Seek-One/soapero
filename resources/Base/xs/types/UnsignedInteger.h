/*
 * UnsignedInteger.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XS_UNSIGNEDINTEGER_H_
#define XS_UNSIGNEDINTEGER_H_

#include <QDomElement>
#include <QString>

namespace XS {

class UnsignedInteger
{
public:
	UnsignedInteger();
	UnsignedInteger(unsigned int iValue);
	UnsignedInteger(const UnsignedInteger& other);
	virtual ~UnsignedInteger();

	void setValue(unsigned int iValue);
	unsigned int getValue() const;

	UnsignedInteger& operator= (const UnsignedInteger& other);
	bool operator== (const UnsignedInteger& other) const;
	bool operator!= (const UnsignedInteger& other) const;

	UnsignedInteger& operator= (unsigned int iValue);
	bool operator== (unsigned int iValue) const;
	bool operator!= (unsigned int iValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	unsigned int m_iValue;
	bool m_bIsNull;

};

}

#endif /* XS_UNSIGNEDINTEGER_H_ */
