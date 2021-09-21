//
// Created by ebeuque on 20/09/2021.
//

#include "StringUtils.h"

#include "ModelUtils.h"

ModelUtils::ModelUtils()
{

}

ModelUtils::~ModelUtils()
{

}

QString ModelUtils::getCapitalizedName(const QString& szName)
{
	QString szTmp;
	szTmp = szName.left(1).toUpper() + szName.mid(1);
	return StringUtils::secureString(szTmp);
}

QString ModelUtils::getUncapitalizedName(const QString& szName)
{
	QString szTmp;
	szTmp = szName.left(1).toLower() + szName.mid(1);
	return StringUtils::secureString(szTmp);
}

QString ModelUtils::getCPPTypeName(const QString& szNamespace, const QString& szLocalName)
{
	QString szTypeName;

	if(!szNamespace.isEmpty()){
		szTypeName += szNamespace.toUpper() + "::";
	}
	szTypeName += szLocalName;

	return szTypeName;
}