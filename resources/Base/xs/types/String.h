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
	virtual ~String();

	void setValue(const QString& szValue);
	const QString& getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	QString m_szValue;

};

}
#endif /* XS_STRING_H_ */
