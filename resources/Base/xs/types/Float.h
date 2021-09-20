/*
 * XSFloat.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSFLOAT_H_
#define SRC_BASE_XSFLOAT_H_

#include <QDomElement>

namespace XS {

class Float {
public:
	Float();
	virtual ~Float();

	void setValue(float fValue);
	float getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	float m_fValue;

	bool m_bIsNull;
};

}
#endif /* SRC_BASE_XSFLOAT_H_ */
