/*
 * XSDuration.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSDURATION_H_
#define XSDURATION_H_

#include <QDomElement>
#include <QString>

namespace Onvif {
namespace XS {

class Duration
{
public:
	Duration();
	virtual ~Duration();

	void setValue(const QString& szValue);
	QString getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);

	bool isNull() const;

private:
	QString m_szValue;

	bool m_bIsNull;

};

}
}

#endif /* XSDURATION_H_ */
