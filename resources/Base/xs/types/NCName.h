/*
 * XSNCName.h
 *
 *  Created on: 22 juin 2018
 *      Author: alavier
 */

#ifndef SRC_BASE_XSNCNAME_H_
#define SRC_BASE_XSNCNAME_H_

#include "String.h"

namespace XS {

class NCName : public String {
public:
	NCName();
	virtual ~NCName();
};

}
#endif /* SRC_BASE_XSNCNAME_H_ */
