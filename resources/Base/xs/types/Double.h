/*
 * Double.h
 *
 *  Created on: 11 juil. 2018
 *      Author: alavier
 */

#ifndef XS_DOUBLE_H_
#define XS_DOUBLE_H_

#include <QDomElement>

namespace XS {

class Double {
public:
	Double();
	Double(double fValue);
	Double(const Double& other);
	virtual ~Double();

	void setValue(double fValue);
	double getValue() const;

	Double& operator= (const Double& other);
	bool operator== (const Double& other) const;
	bool operator!= (const Double& other) const;

	Double& operator= (double fValue);
	bool operator== (double fValue) const;
	bool operator!= (double fValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	double m_fValue;
	bool m_bIsNull;
};

}
#endif /* XS_DOUBLE_H_ */
