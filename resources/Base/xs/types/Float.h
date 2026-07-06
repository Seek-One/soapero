/*
 * Float.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef XS_FLOAT_H_
#define XS_FLOAT_H_

#include <QDomElement>

namespace XS {

class Float {
public:
	Float();
	Float(float fValue);
	Float(const Float& other);
	virtual ~Float();

	void setValue(float fValue);
	float getValue() const;

	Float& operator= (const Float& other);
	bool operator== (const Float& other) const;
	bool operator!= (const Float& other) const;

	Float& operator= (float fValue);
	bool operator== (float fValue) const;
	bool operator!= (float fValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	float m_fValue;

	bool m_bIsNull;
};

}
#endif /* XS_FLOAT_H_ */
