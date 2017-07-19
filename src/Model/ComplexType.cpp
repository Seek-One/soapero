/*
 * ComplexType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "ComplexType.h"


Element::Element()
{

}

Element::~Element()
{

}

ElementSharedPtr Element::create()
{
	return ElementSharedPtr(new Element());
}

void Element::setName(const QString& szName)
{
	m_szName = szName;
}

QString Element::getName() const
{
	return m_szName;
}

void Element::setType(const TypeSharedPtr& pType)
{
	m_pType = pType;
}

TypeSharedPtr Element::getType() const
{
	return m_pType;
}

ElementList::ElementList()
{

}

ElementList::~ElementList()
{

}

ElementListSharedPtr ElementList::create()
{
	return ElementListSharedPtr(new ElementList());
}

Attribute::Attribute()
{
	m_bRequired = false;
}

Attribute::~Attribute()
{

}

AttributeSharedPtr Attribute::create()
{
	return AttributeSharedPtr(new Attribute());
}

void Attribute::setName(const QString& szName)
{
	m_szName = szName;
}

QString Attribute::getName() const
{
	return m_szName;
}

void Attribute::setType(const TypeSharedPtr& pType)
{
	m_pType = pType;
}

TypeSharedPtr Attribute::getType() const
{
	return m_pType;
}

void Attribute::setRequired(bool bRequired)
{
	m_bRequired = bRequired;
}

bool Attribute::isRequired() const
{
	return m_bRequired;
}


AttributeList::AttributeList()
	:QList<AttributeSharedPtr>()
{

}

AttributeList::~AttributeList()
{

}

AttributeListSharedPtr AttributeList::create()
{
	return AttributeListSharedPtr(new AttributeList());
}

/*********************************************************
 *  COMPLEX TYPE
 */

ComplexType::ComplexType()
	:Type(Type::ComplexType)
{
	m_pListAttribute = AttributeList::create();
	m_pListElement = ElementList::create();
}

ComplexType::~ComplexType()
{

}

ComplexTypeSharedPtr ComplexType::create()
{
	return ComplexTypeSharedPtr(new ComplexType());
}

void ComplexType::setExtensionType(TypeSharedPtr pType)
{
	m_pExtensionType = pType;
}

TypeSharedPtr ComplexType::getExtensionType() const
{
	return m_pExtensionType;
}

void ComplexType::addAttribute(const AttributeSharedPtr& pAttribute)
{
	m_pListAttribute->append(pAttribute);
}

AttributeListSharedPtr ComplexType::getAttributeList() const
{
	return m_pListAttribute;
}

void ComplexType::addElement(const ElementSharedPtr& pElement)
{
	m_pListElement->append(pElement);
}

ElementListSharedPtr ComplexType::getElementList() const
{
	return m_pListElement;
}

QString ComplexType::getSetterDeclaration() const
{
	QString szNameCapitalized = m_szName.left(1).toUpper()+m_szName.mid(1);
	QString szVarName = m_szName.left(1).toLower()+m_szName.mid(1);

	QString szDeclaration;
	szDeclaration += "void set";
	szDeclaration += szNameCapitalized;
	szDeclaration += "(";
	szDeclaration += "const ";
	szDeclaration += szNameCapitalized;
	szDeclaration += "& ";
	szDeclaration += szVarName;
	szDeclaration += ");";

	return szDeclaration;
}

QString ComplexType::getGetterDeclaration() const
{
	QString szNameCapitalized = m_szName.left(1).toUpper()+m_szName.mid(1);
	QString szVarName = m_szName.left(1).toLower()+m_szName.mid(1);

	QString szDeclaration;
	szDeclaration += szNameCapitalized;
	szDeclaration += " get";
	szDeclaration += szNameCapitalized;
	szDeclaration += "() const;";

	return szDeclaration;
}

QString ComplexType::getVariableDeclaration() const
{
	QString szNameCapitalized = m_szName.left(1).toUpper()+m_szName.mid(1);
	QString szVarName = m_szName.left(1).toLower()+m_szName.mid(1);

	QString szDeclaration;
	szDeclaration += szNameCapitalized;
	szDeclaration += " _";
	szDeclaration += szVarName;

	return szDeclaration;
}

ComplexTypeList::ComplexTypeList()
	:QList<ComplexTypeSharedPtr>()
{

}

ComplexTypeList::~ComplexTypeList()
{

}

ComplexTypeListSharedPtr ComplexTypeList::create()
{
	return ComplexTypeListSharedPtr(new ComplexTypeList());
}

