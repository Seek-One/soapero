/*
 * XSQName.h
 *
 *  Created on: 21 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSQNAME_H_
#define SRC_BASE_XSQNAME_H_

#include "String.h"

namespace XS {

class QName : public String {
public:
	QName();
	virtual ~QName();
};

}
#endif /* SRC_BASE_XSQNAME_H_ */
