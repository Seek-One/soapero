/*
 * StringUtils.h
 *
 *  Created on: 2 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_UTILS_STRINGUTILS_H_
#define SRC_UTILS_STRINGUTILS_H_

#include <QRegExp>
#include <QString>

class StringUtils {
public:
	static inline QString removeNonAlphaNum(const QString& szString)
	{
		QString szRet = szString;
		szRet.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
		return szRet;
	}
};

#endif /* SRC_UTILS_STRINGUTILS_H_ */
