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

	void setRef(const ElementSharedPtr& pRef);
	ElementSharedPtr getRef() const;
	bool hasRef() const;
	bool needRef() const;

	void setRefValue(const QString& szRefValue);
	const QString& getRefValue() const;

	void setNamespace(const QString& szNamespace);
	const QString& getNamespace() const;

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

	void setIsNested(bool bIsNested);
	bool isNested() const;
	void setIsPointer(bool bIsPointer);
	bool isPointer() const;

	void setMinOccurs(int iMinOccurs);
	int getMinOccurs()const;

	void setMaxOccurs(int iMaxOccurs);
	int getMaxOccurs()const;

	QString getVariableName() const;
	QString getTagQualifiedName() const;

	QString getSetterDeclaration() const;
	QString getGetterDeclaration() const;
	QString getSerializerDeclaration() const;
	QString getDeserializerDeclaration() const;
	QString getVariableDeclaration() const;

	QString getSetterDefinition(const QString& szClassname) const;
	QString getGetterDefinition(const QString& szClassname) const;
	QString getSerializerDefinition(const QString& szClassname, const QString& szNamespace) const;
	QString getDeserializerDefinition(const QString& szClassname) const;

private:
	ElementSharedPtr m_pRef;
	QString m_szRefValue;
	QString m_szNamespace;

	QString m_szName;
	TypeSharedPtr m_pType;
	bool m_bIsNested;
	bool m_bIsPointer;	// Difference with "isNested" is that "isPointer" means need at least a class declaration in the .h and an "#include" in the .cpp

	int m_iMinOccurs;
	int m_iMaxOccurs;

};

class ElementList : public QList<ElementSharedPtr>
{
public:
	ElementList();
	virtual ~ElementList();

	static ElementListSharedPtr create();

	ElementSharedPtr getByRef(const QString& szRef) const;
};

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

	QString getSetterDeclaration() const;
	QString getGetterDeclaration() const;
	QString getVariableDeclaration() const;

	QString getSetterDefinition(const QString& szClassname) const;
	QString getGetterDefinition(const QString& szClassname) const;

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

class ComplexType : public Type
{
public:
	ComplexType();
	virtual ~ComplexType();

	static ComplexTypeSharedPtr create();

	void setExtensionType(TypeSharedPtr pType, bool bIsList = false);
	TypeSharedPtr getExtensionType() const;
	bool isExtensionTypeList() const;

	void setIsSoapEnvelopeFault(bool bIsSoapEnvelopeFault);
	bool isSoapEnvelopeFault() const;

	void addAttribute(const AttributeSharedPtr& pAttribute);
	AttributeListSharedPtr getAttributeList() const;

	void addElement(const ElementSharedPtr& pElement);
	ElementListSharedPtr getElementList() const;

	QString getSetterDeclaration(const QString& szName = QString()) const;
	QString getGetterDeclaration(const QString& szName = QString()) const;
	QString getSerializerDeclaration() const;
	QString getDeserializerDeclaration() const;
	QString getVariableDeclaration(const QString& szName = QString()) const;
	QString getIsNullDeclaration() const;

	QString getSetterDefinition(const QString& szClassname, const QString& szName = QString()) const;
	QString getGetterDefinition(const QString& szClassname, const QString& szName = QString()) const;
	QString getSerializerDefinition(const QString& szClassname, const QString& szNamespace = QString()) const;
	QString getDeserializerDefinition(const QString& szClassname) const;
	QString getIsNullDefinition(const QString& szClassname) const;

	QString getVariableName() const;

private:
	AttributeListSharedPtr m_pListAttribute;
	ElementListSharedPtr m_pListElement;

	TypeSharedPtr m_pExtensionType;
	bool m_bIsListExtension;

	bool m_bIsSoapEnvelopeFault;
};

class ComplexTypeList : public QList<ComplexTypeSharedPtr>
{
public:
	ComplexTypeList();
	virtual ~ComplexTypeList();

	static ComplexTypeListSharedPtr create();

};

#endif /* COMPLEXTYPE_H_ */
