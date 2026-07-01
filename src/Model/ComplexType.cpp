/*
 * ComplexType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "../Utils/StringUtils.h"
#include "../Utils/ModelUtils.h"

#include "SimpleType.h"
#include "Element.h"
#include "Attribute.h"

#include "ComplexType.h"

#include "../common.h"

ComplexType::ComplexType() : Type(Type::CategoryType, Type::TypeComplex)
{
	m_pListAttribute = AttributeList::create();
	m_pListElement = ElementList::create();

	m_bIsListExtension = false;

	m_bIsSoapEnvelopeFault = false;
}

ComplexType::ComplexType(const ComplexType& other)
	: Type(other)
{
	m_pListAttribute = other.m_pListAttribute;
	m_pListElement = other.m_pListElement;

	m_pExtensionType = other.m_pExtensionType;
	m_bIsListExtension = other.m_bIsListExtension;

	m_bIsSoapEnvelopeFault = other.m_bIsSoapEnvelopeFault;
}

ComplexType::~ComplexType()
{

}

ComplexTypeSharedPtr ComplexType::create()
{
	return ComplexTypeSharedPtr(new ComplexType());
}

ComplexTypeSharedPtr ComplexType::clone()
{
	return ComplexTypeSharedPtr(new ComplexType(*this));
}

void ComplexType::setExtensionType(TypeSharedPtr pType, bool bIsList)
{
	m_pExtensionType = pType;
	m_bIsListExtension = bIsList;
}

TypeSharedPtr ComplexType::getExtensionType() const
{
	return m_pExtensionType;
}

bool ComplexType::isExtensionTypeList() const
{
	return m_bIsListExtension;
}

void ComplexType::setIsSoapEnvelopeFault(bool bIsSoapEnvelopeFault)
{
	m_bIsSoapEnvelopeFault = bIsSoapEnvelopeFault;
}

bool ComplexType::isSoapEnvelopeFault() const
{
	return m_bIsSoapEnvelopeFault;
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

QString ComplexType::getVariableName() const
{
	return "_" + ModelUtils::getUncapitalizedName(getLocalName());
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
