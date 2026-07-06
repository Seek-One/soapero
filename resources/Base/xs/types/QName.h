/*
 * QName.h
 *
 *  Created on: 21 juin 2018
 *      Author: alavier
 */

#ifndef XS_QNAME_H_
#define XS_QNAME_H_

#include "String.h"

namespace XS {

class QName : public String {
public:
	QName();
	QName(const QString& szValue);
	QName(const QName& other);
	virtual ~QName();

	QName& operator= (const QName& other);
	QName& operator= (const char* szValue);
	QName& operator= (const QString& szValue);
};

}
#endif /* XS_QNAME_H_ */
