/*
 * XSDouble.h
 *
 *  Created on: 11 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSDOUBLE_H_
#define SRC_BASE_XSDOUBLE_H_

#include <QDomElement>

namespace XS {

class Double {
public:
	Double();
	virtual ~Double();

	void setValue(double fValue);
	double getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	double m_fValue;
	bool m_bIsNull;
};

}
#endif /* SRC_BASE_XSDOUBLE_H_ */
