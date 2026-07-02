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
	ClassCategory getClassCategory() const;

	// Attributes
	QString getLocalName(bool bSafe = false) const;
	void setLocalName(const QString& szLocalName);

	const QString& getNamespace() const;
	void setNamespace(const QString& szNamespace);

	const QString& getNamespaceUri() const;
	void setNamespaceUri(const QString& szNamespaceUri);

	const QString& getSchemaUri() const;
	void setSchemaUri(const QString& szSchemaUri);

	// Utility function
	void setName(const QString& szName);
	QString getQualifiedName() const;
	void setQualifedName(const QString& szNamespace, const QString& szName);

	QString getTagQualifiedName() const;
	QString getNameWithNamespace() const;


protected:
	QString getCategoryNamespace() const;

private:
	ClassCategory m_iCategory;
	QString m_szLocalName;
	QString m_szNamespace;
	QString m_szNamespaceUri;
	QString m_szSchemaUri;
};



#endif /* CLASSNAME_H_ */
