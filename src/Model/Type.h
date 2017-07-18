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

private:
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

class SimpleType : public Type
{
public:
	enum VariableType {
		Unknown,
		string,
		unsignedInt,
		Boolean,
	};

	SimpleType();
	virtual ~SimpleType();

	static SimpleTypeSharedPtr create();

	void setVariableType(VariableType type);
	void setVariableTypeFromString(const QString& szType);
	VariableType getVariableType()const;
	QString getVariableTypeString()const;

	void setMaxLength(int iMaxLength);
	int getMaxLength()const;

	void setMinLength(int iMinLength);
	int getMinLength()const;

	void setMinInclusive(int iMinInclusive);
	int getMinInclusive()const;

private:
	VariableType m_variableType;

	//For base string
	int m_iMaxLength;
	int m_iMinLength;

	//For base unsigned int
	int m_iMinInclusive;

};

class SimpleTypeList : public QList<SimpleTypeSharedPtr>
{
public:
	SimpleTypeList();
	virtual ~SimpleTypeList();

	static SimpleTypeListSharedPtr create();
};

class Attribute
{
public:
	Attribute();
	virtual ~Attribute();

	static AttributeSharedPtr create();

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

	void setRequired(bool bRequired);
	bool isRequired() const;

private:
	QString m_szName;
	TypeSharedPtr m_pType;
	bool m_bRequired;

};

class AttributeList : public QList<AttributeSharedPtr>
{
public:
	AttributeList();
	virtual ~AttributeList();

	static AttributeListSharedPtr create();

};

class ComplexType : public Type
{
public:
	ComplexType();
	virtual ~ComplexType();

	static ComplexTypeSharedPtr create();

	void setExtensionType(TypeSharedPtr pType);
	TypeSharedPtr getExtensionType() const;

	void addAttribute(const AttributeSharedPtr& pAttribute);
	AttributeListSharedPtr getAttributeList() const;

private:
	AttributeListSharedPtr m_pListAttribute;

	TypeSharedPtr m_pExtensionType;
};

class ComplexTypeList : public QList<ComplexTypeSharedPtr>
{
public:
	ComplexTypeList();
	virtual ~ComplexTypeList();

	static ComplexTypeListSharedPtr create();

};

#endif /* TYPE_H_ */
