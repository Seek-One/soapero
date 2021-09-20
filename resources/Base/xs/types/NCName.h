/*
 * NCName.h
 *
 *  Created on: 22 juin 2018
 *      Author: alavier
 */

#ifndef XS_NCNAME_H_
#define XS_NCNAME_H_

#include "String.h"

namespace XS {

class NCName : public String {
public:
	NCName();
	virtual ~NCName();
};

}
#endif /* XS_NCNAME_H_ */
