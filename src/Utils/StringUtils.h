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
#include <QStringList>

class StringUtils {
public:
	static inline QString removeNonAlphaNum(const QString& szString)
	{
		QString szRet = szString;
		QString szChars = "-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\";
		szRet.remove(QRegExp(QString::fromUtf8("[" + szChars + "]")));
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
