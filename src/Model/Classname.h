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
	enum ClassCategory {
		CategoryUnknown,
		CategoryType,
		CategoryMessage
	};

public:
	Classname(ClassCategory iCategory);
	Classname(const Classname& other);
	virtual ~Classname();

public:
	void setQualifedName(const QString& szNamespace, const QString& szName);
	void setNamespace(const QString& szNamespace);
	void setNamespaceUri(const QString& szNamespaceUri);
	void setLocalName(const QString& szLocalName);
	void setName(const QString& szName);
	QString getTagQualifiedName() const;
	QString getNameWithNamespace() const;
	QString getQualifiedName() const;
	const QString& getNamespace() const;
	const QString& getNamespaceUri() const;
	QString getLocalName(bool bSafe = false) const;
	QString getName() const;

	ClassCategory getClassCategory() const;

protected:
	QString getCategoryNamespace() const;

private:
	ClassCategory m_iCategory;
	QString m_szLocalName;
	QString m_szNamespace;
	QString m_szNamespaceUri;
};



#endif /* CLASSNAME_H_ */
