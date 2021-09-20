/*
 * Base64Binary.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef XS_BASE64BINARY_H_
#define XS_BASE64BINARY_H_

#include "String.h"

namespace XS {

class Base64Binary : public String {
public:
	Base64Binary();
	virtual ~Base64Binary();
};

}
#endif /* XS_BASE64BINARY_H_ */
