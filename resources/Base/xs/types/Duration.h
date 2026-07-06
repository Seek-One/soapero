/*
 * Duration.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XS_DURATION_H_
#define XS_DURATION_H_

#include <QDomElement>
#include <QString>

namespace XS {

class Duration
{
public:
	Duration();
	Duration(const QString& szValue);
	Duration(const Duration& other);
	virtual ~Duration();

	void setValue(const QString& szValue);
	const QString& getValue() const;

	Duration& operator= (const Duration& other);
	bool operator== (const Duration& other) const;
	bool operator!= (const Duration& other) const;

	Duration& operator= (const char* szValue);
	bool operator== (const char* szValue) const;
	bool operator!= (const char* szValue) const;

	Duration& operator= (const QString& szValue);
	bool operator== (const QString& szValue) const;
	bool operator!= (const QString& szValue) const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	QString m_szValue;

	bool m_bIsNull;

};

}

#endif /* XS_DURATION_H_ */
