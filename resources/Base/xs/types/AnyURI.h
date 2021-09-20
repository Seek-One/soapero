/*
 * XSAnyURI.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSANYURI_H_
#define SRC_BASE_XSANYURI_H_

#include "String.h"

namespace XS {

class AnyURI : public String {
public:
	AnyURI();
	virtual ~AnyURI();
};

}
#endif /* SRC_BASE_XSANYURI_H_ */
