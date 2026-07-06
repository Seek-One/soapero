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
	Base64Binary(const QString& szValue);
	Base64Binary(const Base64Binary& other);
	virtual ~Base64Binary();

	Base64Binary& operator= (const Base64Binary& other);
	Base64Binary& operator= (const QString& szValue);
};

}
#endif /* XS_BASE64BINARY_H_ */
