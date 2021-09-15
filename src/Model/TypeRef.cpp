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

const QString& TypeRef::getTypeName() const
{
	return m_szTypeName;
}

const QString& TypeRef::getNamespace() const
{
	return m_szNamespace;
}

Type::ClassType TypeRef::getClassType() const
{
	if(m_pType){
		return m_pType->getClassType();
	}
	return Type::TypeUnknown;
}

TypeRefSharedPtr TypeRef::createSimpleType()
{
	TypeRefSharedPtr pTypeRef = TypeRefSharedPtr(new TypeRef());
	pTypeRef->m_pType = SimpleType::create();
	return pTypeRef;
}

TypeRefSharedPtr TypeRef::createComplexType()
{
	TypeRefSharedPtr pTypeRef = TypeRefSharedPtr(new TypeRef());
	return pTypeRef;
}

QSharedPointer<SimpleType> TypeRef::getSimpleType() const
{
	QSharedPointer<SimpleType> pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
	return pSimpleType;
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

	qDebug("type count: %d", count());
	TypeRefList::const_iterator iter_type;
	for(iter_type = constBegin(); iter_type != constEnd(); ++iter_type)
	{
		pCurrentType = (*iter_type);
		qDebug("  type: %s:%s (%d)", qPrintable(pCurrentType->getNamespace()), qPrintable(pCurrentType->getTypeName()), pCurrentType->getClassType());
	}
}