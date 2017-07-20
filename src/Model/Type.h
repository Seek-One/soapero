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

	void setQualifedName(const QString& szNamespace, const QString& szName);
	void setNamespace(const QString& szNamespace);
	void setLocalName(const QString& szLocalName);
	void setName(const QString& szName);
	QString getQualifiedName() const;
	QString getNamespace() const;
	QString getLocalName() const;
	QString getName() const;

	void setClassType(ClassType type);
	ClassType getClassType() const;

protected:
	QString m_szLocalName;
	QString m_szNamespace;
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

	TypeSharedPtr getByName(const QString& szLocalName, const QString& szNamespace = QString());
};

#endif /* TYPE_H_ */
