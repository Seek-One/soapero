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

	void setNull();
	bool isNull() const;

	void operator=(const Boolean& other);
	void operator=(bool bOther);
	bool operator==(const Boolean& other) const;
	bool operator==(bool bOther) const;

private:
	short m_iValue;

};

}
}
#endif /* XSBOOLEAN_H_ */
