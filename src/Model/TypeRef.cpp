//
// Created by ebeuque on 15/09/2021.
//

#include "TypeRef.h"

TypeRef::TypeRef()
{

}

TypeRef::~TypeRef()
{

}

TypeRefSharedPtr TypeRef::create()
{
	return TypeRefSharedPtr(new TypeRef());
}

void TypeRef::setTypeName(const QString& szTypeName)
{
	m_szTypeName = szTypeName;
}

const QString& TypeRef::getTypeName() const
{
	return m_szTypeName;
}

void TypeRef::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

const QString& TypeRef::getNamespace() const
{
	return m_szNamespace;
}

TypeRefList::TypeRefList()
{

}

TypeRefList::~TypeRefList()
{

}

TypeRefListSharedPtr TypeRefList::create()
{
	return TypeRefListSharedPtr(new TypeRefList());
}

TypeRefSharedPtr TypeRefList::getByTypeName(const QString& szTypeName, const QString& szNamespace)
{
	TypeRefSharedPtr pCurrentType;

	TypeRefList::const_iterator iter_type;
	for(iter_type = constBegin(); iter_type != constEnd(); ++iter_type)
	{
		pCurrentType = (*iter_type);
		if(pCurrentType->getTypeName() != szTypeName){
			continue;
		}
		if(pCurrentType->getNamespace() != szNamespace){
			continue;
		}
		return pCurrentType;
	}

	return TypeRefSharedPtr();
}

void TypeRefList::print()
{
	TypeRefSharedPtr pCurrentType;

	qDebug("type count: %d", (int)count());
	TypeRefList::const_iterator iter_type;
	for(iter_type = constBegin(); iter_type != constEnd(); ++iter_type)
	{
		pCurrentType = (*iter_type);
		qDebug("  type: %s:%s", qPrintable(pCurrentType->getNamespace()), qPrintable(pCurrentType->getTypeName()));
	}
}