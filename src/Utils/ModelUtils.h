//
// Created by ebeuque on 20/09/2021.
//

#ifndef COM_JET1OEIL_SOAPERO_MODELUTILS_H
#define COM_JET1OEIL_SOAPERO_MODELUTILS_H

#include <QString>

class ModelUtils
{
public:
	ModelUtils();
	virtual ~ModelUtils();

	static QString getCapitalizedName(const QString& szName);
	static QString getUncapitalizedName(const QString& szName);
	static QString getCPPTypeName(const QString& szNamespace, const QString& szLocalName);
};


#endif //COM_JET1OEIL_SOAPERO_MODELUTILS_H
