/*
 * Type.cpp
 *
 *  Created on: 13 juil. 2017
 *      Author: lgruber
 */

#include "Type.h"

Type::Type(ClassType type)
{
	m_classType = type;
}

Type::~Type()
{

}

void Type::setName(const QString& szName)
{
	m_szName = szName;
}

QString Type::getName() const
{
	return m_szName;
}

void Type::setClassType(Type::ClassType type)
{
	m_classType = type;
}

Type::ClassType Type::getClassType() const
{
	return m_classType;
}


TypeList::TypeList()
	:QList<TypeSharedPtr>()
{

}

TypeList::~TypeList()
{

}

TypeListSharedPtr TypeList::create()
{
	return TypeListSharedPtr(new TypeList());
}

TypeSharedPtr TypeList::getByName(const QString szName)
{
	TypeList::const_iterator type;
	for(type = constBegin(); type != constEnd(); ++type) {
		if((*type)->getName() == szName) {
			return *type;
		}
	}
	return TypeSharedPtr();
}

void TypeList::add(const TypeListSharedPtr& pList)
{
	const_iterator type;
	for(type = constBegin(); type != constEnd(); ++type) {
		QList<TypeSharedPtr>::append(*type);
	}
}

void TypeList::add(const TypeSharedPtr& pType)
{
	QList<TypeSharedPtr>::append(pType);
}

/*********************************************************
 * SIMPLE TYPE
 */

SimpleType::SimpleType()
	:Type(Type::SimpleType)
{
	m_variableType = Unknown;
	m_iMaxLength = -1;
	m_iMinLength = -1;
	m_iMinInclusive = -1;
}

SimpleType::~SimpleType() {}

SimpleTypeSharedPtr SimpleType::create()
{
	return SimpleTypeSharedPtr(new SimpleType());
}

void SimpleType::setVariableType(VariableType type)
{
	m_variableType = type;
}

void SimpleType::setVariableTypeFromString(const QString& szType)
{
	if(szType == "xs:string") {
		m_variableType = string;
	} else if(szType == "xs:unsignedInt") {
		m_variableType = unsignedInt;
	} else if(szType == "xs:boolean") {
		m_variableType = Boolean;
	}
}

SimpleType::VariableType SimpleType::getVariableType()const
{
	return m_variableType;
}

QString SimpleType::getVariableTypeString()const
{
	switch(m_variableType) {
	case string:
		return "std::string";
		break;
	case unsignedInt:
		return "unsigned int";
		break;
	case Boolean:
		return "bool";
		break;
	default:
		return QString();
	}
}

void SimpleType::setMaxLength(int iMaxLength)
{
	m_iMaxLength = iMaxLength;
}

int SimpleType::getMaxLength() const
{
	return m_iMaxLength;
}


void SimpleType::setMinLength(int iMinLength)
{
	m_iMinLength = iMinLength;
}

int SimpleType::getMinLength() const
{
	return m_iMinLength;
}

void SimpleType::setMinInclusive(int iMinInclusive)
{
	m_iMinInclusive = iMinInclusive;
}

int SimpleType::getMinInclusive() const
{
	return m_iMinInclusive;
}

SimpleTypeList::SimpleTypeList()
	:QList<SimpleTypeSharedPtr>()
{

}

SimpleTypeList::~SimpleTypeList()
{

}

SimpleTypeListSharedPtr SimpleTypeList::create()
{
	return SimpleTypeListSharedPtr(new SimpleTypeList());
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
