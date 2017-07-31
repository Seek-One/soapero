/*
 * ComplexType.h
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#ifndef COMPLEXTYPE_H_
#define COMPLEXTYPE_H_

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Type.h"

class Element
{
public:
	Element();
	virtual ~Element();

	static ElementSharedPtr create();

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

private:
	QString m_szName;
	TypeSharedPtr m_pType;

};

class ElementList : public QList<ElementSharedPtr>
{
public:
	ElementList();
	virtual ~ElementList();

	static ElementListSharedPtr create();

};

class Attribute
{
public:
	Attribute();
	virtual ~Attribute();

	static AttributeSharedPtr create();

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

	void setRequired(bool bRequired);
	bool isRequired() const;

private:
	QString m_szName;
	TypeSharedPtr m_pType;
	bool m_bRequired;

};

class AttributeList : public QList<AttributeSharedPtr>
{
public:
	AttributeList();
	virtual ~AttributeList();

	static AttributeListSharedPtr create();

};

class ComplexType : public Type
{
public:
	ComplexType();
	virtual ~ComplexType();

	static ComplexTypeSharedPtr create();

	void setExtensionType(TypeSharedPtr pType);
	TypeSharedPtr getExtensionType() const;

	void addAttribute(const AttributeSharedPtr& pAttribute);
	AttributeListSharedPtr getAttributeList() const;

	void addElement(const ElementSharedPtr& pElement);
	ElementListSharedPtr getElementList() const;

	QString getSetterDeclaration() const;
	QString getGetterDeclaration() const;
	QString getSerializerDeclaration() const;
	QString getVariableDeclaration() const;

	QString getSetterDefinition(const QString& szClassname) const;
	QString getGetterDefinition(const QString& szClassname) const;
	QString getSerializerDefinition(const QString& szClassname) const;

	QString getVariableName() const;

private:
	AttributeListSharedPtr m_pListAttribute;
	ElementListSharedPtr m_pListElement;

	TypeSharedPtr m_pExtensionType;
};

class ComplexTypeList : public QList<ComplexTypeSharedPtr>
{
public:
	ComplexTypeList();
	virtual ~ComplexTypeList();

	static ComplexTypeListSharedPtr create();

};

#endif /* COMPLEXTYPE_H_ */
