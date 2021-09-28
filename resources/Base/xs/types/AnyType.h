/*
 * AnyType.h
 *
 *  Created on: 5 juil. 2018
 *      Author: alavier
 */

#ifndef XS_ANYTYPE_H_
#define XS_ANYTYPE_H_

#include "String.h"

namespace XS {

class AnyType : public String {
public:
	AnyType();
	virtual ~AnyType();
};

}
#endif /* XS_ANYTYPE_H_ */
