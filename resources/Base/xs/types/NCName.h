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
	NCName(const QString& szValue);
	NCName(const NCName& other);
	virtual ~NCName();

	NCName& operator= (const NCName& other);
	NCName& operator= (const char* szValue);
	NCName& operator= (const QString& szValue);
};

}
#endif /* XS_NCNAME_H_ */
