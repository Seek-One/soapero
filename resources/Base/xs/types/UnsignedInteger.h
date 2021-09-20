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
	virtual ~UnsignedInteger();

	void setValue(unsigned int iValue);
	unsigned int getValue() const;

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
