//
// Created by ebeuque on 15/09/2021.
//

#ifndef COM_JET1OEIL_SOAPERO_TYPEREF_H
#define COM_JET1OEIL_SOAPERO_TYPEREF_H

#include <QSharedPointer>

#include "Type.h"
#include "SimpleType.h"

class TypeRef;
typedef QSharedPointer<TypeRef> TypeRefSharedPtr;
class TypeRefList;
typedef QSharedPointer<TypeRefList> TypeRefListSharedPtr;

class TypeRef
{
public:
	TypeRef();
	virtual ~TypeRef();

	const QString& getTypeName() const;
	const QString& getNamespace() const;

	Type::ClassType getClassType() const;

	static TypeRefSharedPtr createSimpleType();
	static TypeRefSharedPtr createComplexType();

	QSharedPointer<SimpleType> getSimpleType() const;

private:
	QString m_szTypeName;
	QString m_szNamespace;

	QSharedPointer<Type> m_pType;
};

class TypeRefList : public QList<TypeRefSharedPtr>
{
public:
	TypeRefList();
	virtual ~TypeRefList();

	static TypeRefListSharedPtr create();

	TypeRefSharedPtr getByTypeName(const QString& szTypeName, const QString& szNamespace);

	void print();
};


#endif //COM_JET1OEIL_SOAPERO_TYPEREF_H
