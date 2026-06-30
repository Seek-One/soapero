//
// Created by ebeuque on 30/06/2026.
//

#ifndef SOAPERO_ELEMENT_H
#define SOAPERO_ELEMENT_H

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Type.h"

class Element
{
public:
	Element();
	virtual ~Element();

	static ElementSharedPtr create();

	void setRef(const ElementSharedPtr& pRef);
	ElementSharedPtr getRef() const;
	bool hasRef() const;
	bool needRef() const;

	void setRefValue(const QString& szRefValue);
	const QString& getRefValue() const;

	void setNamespace(const QString& szNamespace);
	const QString& getNamespace() const;

	void setName(const QString& szName);
	QString getName() const;

	void setType(const TypeSharedPtr& pType);
	TypeSharedPtr getType()const;

	void setIsNested(bool bIsNested);
	bool isNested() const;
	void setIsPointer(bool bIsPointer);
	bool isPointer() const;

	void setMinOccurs(int iMinOccurs);
	int getMinOccurs()const;

	void setMaxOccurs(int iMaxOccurs);
	int getMaxOccurs()const;

	QString getVariableName() const;
	QString getVariableNameList() const;
	QString getTagQualifiedName() const;

	QString getSetterDeclaration() const;
	QString getGetterDeclaration() const;
	QString getVariableDeclaration() const;

	QString getSetterDefinition(const QString& szClassname) const;
	QString getGetterDefinition(const QString& szClassname) const;
	QString getSerializerDefinition(const QString& szClassname, const QString& szNamespace) const;
	QString getDeserializerDefinition(const QString& szClassname) const;

private:
	ElementSharedPtr m_pRef;
	QString m_szRefValue;
	QString m_szNamespace;

	QString m_szName;
	TypeSharedPtr m_pType;
	bool m_bIsNested;
	bool m_bIsPointer;	// Difference with "isNested" is that "isPointer" means need at least a class declaration in the .h and an "#include" in the .cpp

	int m_iMinOccurs;
	int m_iMaxOccurs;
};

class ElementList : public QList<ElementSharedPtr>
{
public:
	ElementList();
	virtual ~ElementList();

	static ElementListSharedPtr create();

	ElementSharedPtr getByRef(const QString& szRef) const;
};

#endif //SOAPERO_ELEMENT_H