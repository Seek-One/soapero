/*
 * XSToken.h
 *
 *  Created on: 26 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSTOKEN_H_
#define SRC_BASE_XSTOKEN_H_

#include "XSString.h"

namespace XS {

class Token : public String {
public:
	Token();
	virtual ~Token();
};

}
#endif /* SRC_BASE_XSTOKEN_H_ */
