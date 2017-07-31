/*
 * XSString.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSSTRING_H_
#define XSSTRING_H_

#include <QString>

class XSString
{
public:
	XSString();
	virtual ~XSString();

	void setValue(const QString& szValue);
	QString getValue() const;

	QString serialize() const;

private:
	QString m_szValue;

};

#endif /* XSSTRING_H_ */
