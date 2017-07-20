/*
 * SimpleType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "SimpleType.h"

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
		return "QString";
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

QString SimpleType::getSetterDeclaration() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration;
	szDeclaration += "void set";
	szDeclaration += getLocalName();
	szDeclaration += "(";
	if(getVariableTypeString().startsWith('Q')) {
		szDeclaration += "const ";
		szDeclaration += getVariableTypeString();
		szDeclaration += "& ";
	}else{
		szDeclaration += getVariableTypeString();
		szDeclaration += " ";
	}
	szDeclaration += szVarName;
	szDeclaration += ");";

	return szDeclaration;
}

QString SimpleType::getGetterDeclaration() const
{
	QString szDeclaration;
	szDeclaration += getVariableTypeString();
	szDeclaration += " get";
	szDeclaration += getLocalName();
	szDeclaration += "() const;";

	return szDeclaration;
}

QString SimpleType::getVariableDeclaration() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration;
	szDeclaration += getVariableTypeString();
	szDeclaration += " _";
	szDeclaration += szVarName;
	szDeclaration += ";";

	return szDeclaration;
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


