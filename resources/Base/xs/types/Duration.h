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
	virtual ~Duration();

	void setValue(const QString& szValue);
	const QString& getValue() const;

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
