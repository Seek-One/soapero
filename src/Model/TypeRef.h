//
// Created by ebeuque on 15/09/2021.
//

#ifndef SOAPERO_MODEL_TYPEREF_H
#define SOAPERO_MODEL_TYPEREF_H

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Type.h"

class TypeRef;
typedef QSharedPointer<TypeRef> TypeRefSharedPtr;
class TypeRefList;
typedef QSharedPointer<TypeRefList> TypeRefListSharedPtr;

class TypeRef
{
public:
	TypeRef();
	virtual ~TypeRef();

	static TypeRefSharedPtr create();

	void setTypeName(const QString& szTypeName);
	const QString& getTypeName() const;

	void setNamespace(const QString& szNamespace);
	const QString& getNamespace() const;

private:
	QString m_szTypeName;
	QString m_szNamespace;
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


#endif //SOAPERO_MODEL_TYPEREF_H
