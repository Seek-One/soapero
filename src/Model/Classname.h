/*
 * Classname.h
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#ifndef CLASSNAME_H_
#define CLASSNAME_H_

#include <QString>

class Classname
{
public:
	Classname();
	virtual ~Classname();

	void setQualifedName(const QString& szNamespace, const QString& szName);
	void setNamespace(const QString& szNamespace);
	void setLocalName(const QString& szLocalName);
	void setName(const QString& szName);
	QString getTagQualifiedName() const;
	QString getQualifiedName() const;
	QString getNamespace() const;
	QString getLocalName() const;
	QString getName() const;

private:
	QString m_szLocalName;
	QString m_szNamespace;

};



#endif /* CLASSNAME_H_ */
