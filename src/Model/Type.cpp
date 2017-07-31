/*
 * Type.cpp
 *
 *  Created on: 13 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "Type.h"

Type::Type(ClassType type)
	:Classname()
{
	m_classType = type;
}

Type::~Type()
{

}

TypeSharedPtr Type::create()
{
	return TypeSharedPtr(new Type(TypeUnknown));
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
