/*
 * XSInteger.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSINTEGER_H_
#define XSINTEGER_H_

#include <QString>

namespace Onvif {
namespace XS {

class Integer
{
public:
	Integer();
	virtual ~Integer();

	void setValue(int iValue);
	int getValue() const;

	QString serialize() const;

	bool isNull() const;

private:
	int m_iValue;

	bool m_bIsNull;

};

}
}

#endif /* XSINTEGER_H_ */
