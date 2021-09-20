/*
 * NonNegativeInteger.h
 *
 *  Created on: 10 juil. 2018
 *      Author: alavier
 */

#ifndef XS_NONNEGATIVEINTEGER_H_
#define XS_NONNEGATIVEINTEGER_H_

#include "UnsignedInteger.h"

namespace XS {

class NonNegativeInteger : public UnsignedInteger {
public:
	NonNegativeInteger();
	virtual ~NonNegativeInteger();
};

}
#endif /* XS_NONNEGATIVEINTEGER_H_ */
