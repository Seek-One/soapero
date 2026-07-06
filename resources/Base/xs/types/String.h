/*
 * String.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XS_STRING_H_
#define XS_STRING_H_

#include <QDomElement>
#include <QString>

namespace XS {

class String
{
public:
	String();
	String(const QString& value);
	virtual ~String();

	void setValue(const QString& value);
	const QString& getValue() const;

	String& operator= (const String& other);
	bool operator== (const String& other) const;
	bool operator!= (const String& other) const;

	String& operator= (const QString& szValue);
	bool operator== (const QString& szValue) const;
	bool operator!= (const QString& szValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	QString m_szValue;

};

}
#endif /* XS_STRING_H_ */
