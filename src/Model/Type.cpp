/*
 * Type.cpp
 *
 *  Created on: 13 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "Type.h"

Type::Type(ClassCategory iCategory, TypeMode iTypeMode) : Classname(iCategory)
{
	m_iTypeMode = iTypeMode;
}

Type::Type(const Type& other) : Classname(other)
{
	m_iTypeMode = other.m_iTypeMode;
}

Type::~Type()
{

}

TypeSharedPtr Type::create()
{
	return TypeSharedPtr(new Type(CategoryType, TypeUnknown));
}

void Type::setTypeMode(Type::TypeMode iTypeMode)
{
	m_iTypeMode = iTypeMode;
}

Type::TypeMode Type::getTypeMode() const
{
	return m_iTypeMode;
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
			if(pCurrentType->getNamespaceUri() != szNamespace){
				continue;
			}
		}
		if(pListIgnoredTypes && pListIgnoredTypes->contains(pCurrentType)){
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
		qDebug("  type: (%d) %s %s", pCurrentType->getTypeMode(), qPrintable(pCurrentType->getLocalName()), qPrintable(pCurrentType->getNamespace()));
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
