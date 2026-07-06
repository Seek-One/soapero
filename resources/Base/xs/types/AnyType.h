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
	AnyType(const QString& szValue);
	AnyType(const AnyType& other);
	virtual ~AnyType();

	AnyType& operator= (const AnyType& other);
	AnyType& operator= (const char* szValue);
	AnyType& operator= (const QString& szValue);
};

}
#endif /* XS_ANYTYPE_H_ */
