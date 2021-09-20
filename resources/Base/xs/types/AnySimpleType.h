/*
 * AnySimpleType.h
 *
 *  Created on: 11 juil. 2018
 *      Author: alavier
 */

#ifndef XS_ANYSIMPLETYPE_H_
#define XS_ANYSIMPLETYPE_H_

#include "String.h"

namespace XS {

class AnySimpleType : public String {
public:
	AnySimpleType();
	virtual ~AnySimpleType();
};

}
#endif /* XS_ANYSIMPLETYPE_H_ */
