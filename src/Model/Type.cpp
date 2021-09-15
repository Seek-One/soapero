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

TypeSharedPtr TypeList::getByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes)
{
	TypeSharedPtr pCurrentType;

	TypeList::const_iterator iter_type;
	for(iter_type = constBegin(); iter_type != constEnd(); ++iter_type)
	{
		pCurrentType = (*iter_type);
		if(pCurrentType->getLocalName() != szLocalName){
			continue;
		}
		if(pCurrentType->getNamespace() != szNamespace){
			continue;
		}
		if(pListIgnoredTypes->contains(pCurrentType)){
			continue;
		}
		return pCurrentType;
	}
	return TypeSharedPtr();
}

void TypeList::print()
{
	TypeSharedPtr pCurrentType;

	qDebug("type count: %d", count());
	TypeList::const_iterator iter_type;
	for(iter_type = constBegin(); iter_type != constEnd(); ++iter_type)
	{
		pCurrentType = (*iter_type);
		qDebug("  type: (%d) %s %s", pCurrentType->getClassType(), qPrintable(pCurrentType->getLocalName()), qPrintable(pCurrentType->getNamespace()));
	}
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
