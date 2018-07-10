/*
 * XSDateTime.h
 *
 *  Created on: 20 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSDATETIME_H_
#define SRC_BASE_XSDATETIME_H_

#include <QDomElement>
#include <QDateTime>

namespace XS {

class DateTime {
public:
	DateTime();
	virtual ~DateTime();

	void setValue(const QDateTime& dateTime);
	const QDateTime& getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	QDateTime m_dateTime;
	bool m_bIsNull;
};

}
#endif /* SRC_BASE_XSDATETIME_H_ */
