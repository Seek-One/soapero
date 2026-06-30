/*
 * ComplexType.h
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#ifndef COMPLEXTYPE_H_
#define COMPLEXTYPE_H_

#include <QSharedPointer>
#include <QList>
#include <QString>

#include "Type.h"

class ComplexType : public Type
{
public:
	ComplexType();
	ComplexType(const ComplexType& other);
	virtual ~ComplexType();

	static ComplexTypeSharedPtr create();
	ComplexTypeSharedPtr clone();

	void setExtensionType(TypeSharedPtr pType, bool bIsList = false);
	TypeSharedPtr getExtensionType() const;
	bool isExtensionTypeList() const;

	void setIsSoapEnvelopeFault(bool bIsSoapEnvelopeFault);
	bool isSoapEnvelopeFault() const;

	void addAttribute(const AttributeSharedPtr& pAttribute);
	AttributeListSharedPtr getAttributeList() const;

	void addElement(const ElementSharedPtr& pElement);
	ElementListSharedPtr getElementList() const;

	QString getVariableDeclaration(const QString& szName = QString()) const;
	QString getIsNullDeclaration() const;
	QString getGetNamespaceDeclarationDeclaration() const;

	QString getSetterDefinition(const QString& szClassname, const QString& szName = QString()) const;
	QString getGetterDefinition(const QString& szClassname, const QString& szName = QString()) const;
	QString getSerializerDefinition(const QString& szClassname, const QString& szNamespace = QString()) const;
	QString getDeserializerDefinition(const QString& szClassname) const;
	QString getIsNullDefinition(const QString& szClassname) const;
	QString getGetNamespaceDeclarationDefinition(const QString& szClassname) const;

	QString getVariableName() const;

private:
	AttributeListSharedPtr m_pListAttribute;
	ElementListSharedPtr m_pListElement;

	TypeSharedPtr m_pExtensionType;
	bool m_bIsListExtension;

	bool m_bIsSoapEnvelopeFault;
};

class ComplexTypeList : public QList<ComplexTypeSharedPtr>
{
public:
	ComplexTypeList();
	virtual ~ComplexTypeList();

	static ComplexTypeListSharedPtr create();

};

#endif /* COMPLEXTYPE_H_ */
