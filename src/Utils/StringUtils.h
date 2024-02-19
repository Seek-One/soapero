/*
 * StringUtils.h
 *
 *  Created on: 2 juil. 2018
 *      Author: alavier
 */

#ifndef SRC_UTILS_STRINGUTILS_H_
#define SRC_UTILS_STRINGUTILS_H_

#include <QString>
#include <QStringList>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

class StringUtils {
public:
	static inline QString removeNonAlphaNum(const QString& szString)
	{
		QString szRet = szString;
		//QString szChars = "-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\";

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QRegularExpression re("[^a-zA-Z\\d\\s]");
#else
		QRegExp re("[^a-zA-Z\\d\\s]");
#endif
		szRet.remove(re);
		szRet.replace(" ", "");
		return szRet;
	}

	static inline QString secureString(const QString& szString)
	{
		QString szRet = replaceNonConformCharacters(szString);
		szRet = replaceCppKeyWords(szRet);
		return szRet;
	}

	static inline QString getLocalNameIfPossible(const QString& szName)
	{
		QStringList splitted = szName.split(":");
		if(splitted.size() == 2){
			return splitted[1];
		}
		return szName;
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
