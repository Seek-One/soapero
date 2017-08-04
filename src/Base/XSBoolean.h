/*
 * XSBoolean.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XSBOOLEAN_H_
#define XSBOOLEAN_H_

#include <QString>

namespace Onvif {
namespace XS {

class Boolean
{
public:
	Boolean();
	virtual ~Boolean();

	void setValue(bool bValue);
	bool getValue() const;

	QString serialize() const;

	bool isNull() const;

private:
	bool m_bValue;
	bool m_bIsNull;

};

}
}
#endif /* XSBOOLEAN_H_ */
