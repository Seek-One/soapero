/*
 * XSAnyType.h
 *
 *  Created on: 5 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSANYTYPE_H_
#define SRC_BASE_XSANYTYPE_H_

#include "XSString.h"

namespace XS {

class AnyType : public String {
public:
	AnyType();
	virtual ~AnyType();
};

}
#endif /* SRC_BASE_XSANYTYPE_H_ */
