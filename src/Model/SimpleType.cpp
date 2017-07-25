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
	m_bRestricted = false;
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
		m_variableType = String;
	} else if(szType == "xs:unsignedInt") {
		m_variableType = UnsignedInt;
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
	if(m_bRestricted && m_variableType == String &&
			m_listEnumerationValues.count() > 0) {
		return getLocalName() + "::Values";

	}else{
		switch(m_variableType) {
		case String:
			return "QString";
			break;
		case UnsignedInt:
			return "XSUnsignedInt";
			break;
		case Boolean:
			return "XSBoolean";
			break;
		default:
			return QString();
		}
	}
}

QString SimpleType::getSetterDeclaration() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration;
	szDeclaration += "void set";
	szDeclaration += getLocalName();
	szDeclaration += "(";
	//if(getVariableTypeString().startsWith('Q')) {
		szDeclaration += "const ";
		szDeclaration += getVariableTypeString();
		szDeclaration += "& ";
	/*}else{
		szDeclaration += getVariableTypeString();
		szDeclaration += " ";
	}*/
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

QString SimpleType::getEnumerationDeclaration() const
{
	QString szDeclaration;
	if(m_bRestricted) {
		szDeclaration += "enum Values {";
		for(int i=0; i < m_listEnumerationValues.count(); ++i) {
			if(i==0) {
				szDeclaration += m_listEnumerationValues[i];
			} else {
				szDeclaration += ", " + m_listEnumerationValues[i];
			}
		}
		szDeclaration += "}";
	}
	return szDeclaration;
}

void SimpleType::setRestricted(bool bRestricted)
{
	m_bRestricted = bRestricted;
}

bool SimpleType::isRestricted() const
{
	return m_bRestricted;
}

void SimpleType::addEnumerationValue(const QString& szValue)
{
	m_listEnumerationValues.append(szValue);
}

QStringList SimpleType::getEnumerationValues() const
{
	return m_listEnumerationValues;
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


