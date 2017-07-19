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
