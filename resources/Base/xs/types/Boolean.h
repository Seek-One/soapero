/*
 * Boolean.h
 *
 *  Created on: 27 juil. 2017
 *      Author: lgruber
 */

#ifndef XS_BOOLEAN_H_
#define XS_BOOLEAN_H_

#include <QDomElement>
#include <QDomAttr>
#include <QString>

namespace XS {

class Boolean
{
public:
	Boolean();
	virtual ~Boolean();

	void setValue(bool bValue);
	bool getValue() const;

	QString serialize() const;
	void deserialize(const QDomElement& element);
	void deserialize(const QDomAttr& attr);

	bool isNull() const;

private:
	bool m_bValue;
	bool m_bIsNull;

};

}
#endif /* XS_BOOLEAN_H_ */
