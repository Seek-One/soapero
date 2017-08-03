/*
 * XSDuration.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSDURATION_H_
#define XSDURATION_H_

#include <QString>

namespace Onvif {
namespace XS {

class Duration
{
public:
	Duration();
	virtual ~Duration();

	void setDuration(const QString& szValue);
	QString getValue() const;

	QString serialize() const;

private:
	QString m_szValue;

};

}
}

#endif /* XSDURATION_H_ */
