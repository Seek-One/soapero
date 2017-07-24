/*
 * Type.cpp
 *
 *  Created on: 13 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "Type.h"

Type::Type(ClassType type)
{
	m_classType = type;
}

Type::~Type()
{

}

TypeSharedPtr Type::create()
{
	return TypeSharedPtr(new Type(Unknown));
}

void Type::setQualifedName(const QString& szNamespace, const QString& szLocalName)
{
	m_szNamespace = szNamespace;
	m_szLocalName = szLocalName;
}

void Type::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

void Type::setLocalName(const QString& szLocalName)
{
	m_szLocalName = szLocalName;
}

void Type::setName(const QString& szName)
{
	if(szName.contains(":")) {
		m_szNamespace = szName.split(":")[0];
		m_szLocalName = szName.split(":")[1];
	}else{
		m_szLocalName = szName;
	}
}

QString Type::getQualifiedName() const
{
	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper() + m_szLocalName;
	}
}

QString Type::getNamespace() const
{
	return m_szNamespace;
}

QString Type::getLocalName() const
{
	return m_szLocalName;
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

TypeSharedPtr TypeList::getByName(const QString& szLocalName, const QString& szNamespace)
{
	TypeList::const_iterator type;
	for(type = constBegin(); type != constEnd(); ++type) {
		if((*type)->getLocalName() == szLocalName &&
				(*type)->getNamespace() == szNamespace) {
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
