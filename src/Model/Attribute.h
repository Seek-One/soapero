//
// Created by ebeuque on 30/06/2026.
//

#ifndef SOAPERO_ATTRIBUTE_H
#define SOAPERO_ATTRIBUTE_H

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Type.h"

class Attribute
{
public:
	Attribute();
	virtual ~Attribute();

	static AttributeSharedPtr create();

	void setRef(const AttributeSharedPtr& pRef);
	const AttributeSharedPtr& getRef() const;
	bool hasRef() const;

	void setNamespace(const QString& szNamespace);
	const QString& getNamespace() const;

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

	void setRequired(bool bRequired);
	bool isRequired() const;

	void setIsList(bool bIsList);
	bool isList() const;

	QString getVariableDeclaration() const;

	QString getSetterDefinition(const QString& szClassname) const;
	QString getGetterDefinition(const QString& szClassname) const;

	QString getVariableName() const;
	QString getVariableNameList() const;

private:
	AttributeSharedPtr m_pRef;		//https://www.w3schools.com/xml/el_attribute.asp (see "ref" part)
	QString m_szNamespace;			// Useful to identify the ref

	QString m_szName;
	TypeSharedPtr m_pType;
	bool m_bRequired;
	bool m_bIsList;
};

class AttributeList : public QList<AttributeSharedPtr>
{
public:
	AttributeList();
	virtual ~AttributeList();

	static AttributeListSharedPtr create();

	AttributeSharedPtr getByRef(const QString& szRef) const;
	bool containsListAttribute() const;
};


#endif //SOAPERO_ATTRIBUTE_H