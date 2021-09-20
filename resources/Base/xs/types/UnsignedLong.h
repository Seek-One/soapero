/*
 * XSUnsignedLong.h
 *
 *  Created on: 2 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSUNSIGNEDLONG_H_
#define SRC_BASE_XSUNSIGNEDLONG_H_

#include <QDomElement>

namespace XS {

class UnsignedLong {
public:
	UnsignedLong();
	virtual ~UnsignedLong();

	void setValue(unsigned long iValue);
	unsigned long getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	unsigned long m_iValue;
	bool m_bIsNull;
};

}
#endif /* SRC_BASE_XSUNSIGNEDLONG_H_ */
