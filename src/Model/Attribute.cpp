//
// Created by ebeuque on 30/06/2026.
//


#include "../Utils/StringUtils.h"
#include "../Utils/ModelUtils.h"

#include "SimpleType.h"
#include "ComplexType.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

#include "Attribute.h"

Attribute::Attribute()
{
	m_bRequired = false;
	m_bIsList = false;
}

Attribute::~Attribute()
{

}

AttributeSharedPtr Attribute::create()
{
	return AttributeSharedPtr(new Attribute());
}

void Attribute::setRef(const AttributeSharedPtr& pRef)
{
	m_pRef = pRef;
}

const AttributeSharedPtr& Attribute::getRef() const
{
	return m_pRef;
}

bool Attribute::hasRef() const
{
	return !m_pRef.isNull();
}

void Attribute::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

const QString& Attribute::getNamespace() const
{
	return m_szNamespace;
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

void Attribute::setIsList(bool bIsList)
{
	m_bIsList = bIsList;
}

bool Attribute::isList() const
{
	return m_bIsList;
}

QString Attribute::getVariableDeclaration() const
{
	QString szDeclaration;

	if(m_pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_bIsList){
			szDeclaration += "QList<";
			szDeclaration += pSimpleType->getCPPTypeNameString();
			szDeclaration += "> ";
			szDeclaration +=  getVariableName();
			szDeclaration += "List";
			szDeclaration += ";";
		}else{
			szDeclaration += pSimpleType->getVariableDeclarationForComplexType() ;
		}
	}else if(m_pType->getTypeMode() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_bIsList){
			szDeclaration += "QList<";
			szDeclaration += pComplexType->getNameWithNamespace();
			szDeclaration += "> ";
			szDeclaration += getVariableName();
			szDeclaration += "List";
			szDeclaration += ";";
		}else{
			szDeclaration += pComplexType->getVariableDeclaration(getName());
		}
	}

	return szDeclaration;
}

QString Attribute::getVariableName() const
{
	QString szName = ModelUtils::getUncapitalizedName(getName());
	return StringUtils::secureString(QString("_%0").arg(szName));
}

QString Attribute::getVariableNameList() const
{
	return getVariableName() + "List";
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

AttributeSharedPtr AttributeList::getByRef(const QString& szRef) const
{
	const_iterator iter;
	for(iter = constBegin(); iter != constEnd(); ++iter){
		QString szNamespace = szRef.split(":")[0];
		QString szName = szRef.split(":")[1];

		if(((*iter)->getName() == szName) && ((*iter)->getNamespace() == szNamespace)){
			return *iter;
		}
	}
	return AttributeSharedPtr();
}

bool AttributeList::containsListAttribute() const
{
	const_iterator iter;
	for(iter = constBegin(); iter != constEnd(); ++iter){
		if((*iter)->isList()){
			return true;
		}
	}
	return false;
}