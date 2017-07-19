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

class Type
{
public:
	enum ClassType {
		ComplexType,
		SimpleType,
	};

protected:
	Type(ClassType type);

public:
	virtual ~Type();


	void setName(const QString& szName);
	QString getName() const;

	void setClassType(ClassType type);
	ClassType getClassType() const;

protected:
	QString m_szName;
	ClassType m_classType;
};

class TypeList : public QList<TypeSharedPtr>
{
public:
	TypeList();
	virtual ~TypeList();

	static TypeListSharedPtr create();

	void add(const TypeListSharedPtr& pList);
	void add(const TypeSharedPtr& pType);

	TypeSharedPtr getByName(const QString szName);
};

#endif /* TYPE_H_ */
