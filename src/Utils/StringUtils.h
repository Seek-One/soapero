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
		szRet.replace(" ", "");
		return szRet;
	}

	static inline QString secureString(const QString& szString)
	{
		QString szRet = replaceNonConformCharacters(szString);
		szRet = replaceCppKeyWords(szRet);
		return szRet;
	}

private:
	static inline QString replaceNonConformCharacters(const QString& szString)
	{
		QString szRet = szString;
		szRet.replace("-", "_");
		szRet.replace(".", "");
		return szRet;
	}

	static inline QString replaceCppKeyWords(const QString& szString)
	{
		QString szRet = szString;
		if(szRet == "namespace"){
			szRet = "name_space";
		}
		return szRet;
	}
};

#endif /* SRC_UTILS_STRINGUTILS_H_ */
