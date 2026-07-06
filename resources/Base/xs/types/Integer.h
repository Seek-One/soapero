/*
 * Integer.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XS_INTEGER_H_
#define XS_INTEGER_H_

#include <QDomElement>
#include <QString>

namespace XS {

class Integer
{
public:
	Integer();
	Integer(int iValue);
	Integer(const Integer& other);
	virtual ~Integer();

	void setValue(int iValue);
	int getValue() const;

	Integer& operator= (const Integer& other);
	bool operator== (const Integer& other) const;
	bool operator!= (const Integer& other) const;

	Integer& operator= (int iValue);
	bool operator== (int iValue) const;
	bool operator!= (int iValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	int m_iValue;

	bool m_bIsNull;

};

}

#endif /* XS_INTEGER_H_ */
