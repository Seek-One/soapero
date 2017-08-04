/*
 * XSString.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSSTRING_H_
#define XSSTRING_H_

#include <QString>

namespace Onvif {
namespace XS {

class String
{
public:
	String();
	virtual ~String();

	void setValue(const QString& szValue);
	QString getValue() const;

	QString serialize() const;

	bool isNull() const;

private:
	QString m_szValue;

};

}
}
#endif /* XSSTRING_H_ */
