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
	NonNegativeInteger(unsigned int iValue);
	NonNegativeInteger(const NonNegativeInteger& other);
	virtual ~NonNegativeInteger();

	NonNegativeInteger& operator= (const NonNegativeInteger& other);
	NonNegativeInteger& operator= (unsigned int iValue);
};

}
#endif /* XS_NONNEGATIVEINTEGER_H_ */
