/*
 * AnyURI.h
 *
 *  Created on: 19 juin 2018
 *      Author: alavier
 */

#ifndef XS_ANYURI_H_
#define XS_ANYURI_H_

#include "String.h"

namespace XS {

class AnyURI : public String {
public:
	AnyURI();
	AnyURI(const QString& szValue);
	AnyURI(const AnyURI& other);
	virtual ~AnyURI();

	AnyURI& operator= (const AnyURI& other);
	AnyURI& operator= (const char* szValue);
	AnyURI& operator= (const QString& szValue);
};

}
#endif /* XS_ANYURI_H_ */
