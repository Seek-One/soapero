/*
 * Type.h
 *
 *  Created on: 13 juil. 2017
 *      Author: lgruber
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Model/Classname.h"

class Type;
typedef QSharedPointer<Type> TypeSharedPtr;
class TypeList;
typedef QSharedPointer<TypeList> TypeListSharedPtr;
class SimpleType;
typedef QSharedPointer<SimpleType> SimpleTypeSharedPtr;
class SimpleTypeList;
typedef QSharedPointer<SimpleTypeList> SimpleTypeListSharedPtr;
class Attribute;
typedef QSharedPointer<Attribute> AttributeSharedPtr;
class AttributeList;
typedef QSharedPointer<AttributeList> AttributeListSharedPtr;
class Element;
typedef QSharedPointer<Element> ElementSharedPtr;
class ElementList;
typedef QSharedPointer<ElementList> ElementListSharedPtr;
class ComplexType;
typedef QSharedPointer<ComplexType> ComplexTypeSharedPtr;
class ComplexTypeList;
typedef QSharedPointer<ComplexTypeList> ComplexTypeListSharedPtr;

class Type : public Classname
{
public:
	enum TypeMode {
		TypeUnknown,
		TypeComplex,
		TypeSimple,
	};

protected:
	Type(ClassCategory iCategory, TypeMode iTypeMode);
	Type(const Type& other);

public:
	virtual ~Type();

	static TypeSharedPtr create();

	void setTypeMode(TypeMode iTypeMode);
	TypeMode getTypeMode() const;

protected:
	TypeMode m_iTypeMode;
};

class TypeList : public QList<TypeSharedPtr>
{
public:
	TypeList();
	virtual ~TypeList();

	static TypeListSharedPtr create();

	void add(const TypeListSharedPtr& pList);
	void add(const TypeSharedPtr& pType);

	TypeSharedPtr getByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes);

	void print();
};

#endif /* TYPE_H_ */
