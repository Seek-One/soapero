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
	virtual ~QName();
};

}
#endif /* XS_QNAME_H_ */
