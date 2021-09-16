/*
 * XSNonNegativeInteger.h
 *
 *  Created on: 10 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSNONNEGATIVEINTEGER_H_
#define SRC_BASE_XSNONNEGATIVEINTEGER_H_

#include "XSUnsignedInteger.h"

namespace XS {

class NonNegativeInteger : public UnsignedInteger {
public:
	NonNegativeInteger();
	virtual ~NonNegativeInteger();
};

}
#endif /* SRC_BASE_XSNONNEGATIVEINTEGER_H_ */
