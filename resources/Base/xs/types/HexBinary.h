/*
 * HexBinary.h
 *
 *  Created on: 10 juil. 2018
 *      Author: alavier
 */

#ifndef XS_HEXBINARY_H_
#define XS_HEXBINARY_H_

#include "String.h"

namespace XS {

class HexBinary : public String {
public:
	HexBinary();
	HexBinary(const QString& szValue);
	HexBinary(const HexBinary& other);
	virtual ~HexBinary();

	HexBinary& operator= (const HexBinary& other);
	HexBinary& operator= (const QString& szValue);
};

}
#endif /* XS_HEXBINARY_H_ */
