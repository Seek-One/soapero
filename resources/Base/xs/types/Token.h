/*
 * Token.h
 *
 *  Created on: 26 juin 2018
 *      Author: alavier
 */

#ifndef XS_TOKEN_H_
#define XS_TOKEN_H_

#include "String.h"

namespace XS {

class Token : public String {
public:
	Token();
	virtual ~Token();
};

}
#endif /* XS_TOKEN_H_ */
