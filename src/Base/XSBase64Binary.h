/*
 * XSBase64Binary.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSBASE64BINARY_H_
#define SRC_BASE_XSBASE64BINARY_H_

#include "XSString.h"

namespace XS {

class Base64Binary : public String {
public:
	Base64Binary();
	virtual ~Base64Binary();
};

}
#endif /* SRC_BASE_XSBASE64BINARY_H_ */
