//
// Created by ebeuque on 30/06/2026.
//

#include "Utils/ModelUtils.h"
#include "Utils/StringUtils.h"

#include "SimpleType.h"
#include "ComplexType.h"

#include "Element.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

Element::Element()
{
	m_bIsNested = false;
	m_bIsPointer = false;
	m_iMinOccurs = 1;
	m_iMaxOccurs = 1; // -1 means unbounded
}

Element::~Element()
{

}

ElementSharedPtr Element::create()
{
	return ElementSharedPtr(new Element());
}

void Element::setRef(const ElementSharedPtr& pRef)
{
	m_pRef = pRef;
}

ElementSharedPtr Element::getRef() const
{
	return m_pRef;
}

bool Element::hasRef() const
{
	return !m_pRef.isNull();
}

bool Element::needRef() const
{
	return (!m_szRefValue.isNull() && !hasRef());
}

void Element::setRefValue(const QString& szRefValue)
{
	m_szRefValue = szRefValue;
}

const QString& Element::getRefValue() const
{
	return m_szRefValue;
}

void Element::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

const QString& Element::getNamespace() const
{
	return m_szNamespace;
}

void Element::setName(const QString& szName)
{
	m_szName = szName;
}

QString Element::getName() const
{
	return m_szName;
}

void Element::setType(const TypeSharedPtr& pType)
{
	m_pType = pType;
}

TypeSharedPtr Element::getType() const
{
	return m_pType;
}

void Element::setIsNested(bool bIsNested)
{
	m_bIsNested = bIsNested;
}

bool Element::isNested() const
{
	return m_bIsNested;
}

void Element::setIsPointer(bool bIsPointer)
{
	m_bIsPointer = bIsPointer;
}

bool Element::isPointer() const
{
	return m_bIsPointer;
}

void Element::setMinOccurs(int iMinOccurs)
{
	m_iMinOccurs = iMinOccurs;
}

int Element::getMinOccurs()const
{
	return m_iMinOccurs;
}

void Element::setMaxOccurs(int iMaxOccurs)
{
	m_iMaxOccurs = iMaxOccurs;
}

int Element::getMaxOccurs()const
{
	return m_iMaxOccurs;
}

QString Element::getVariableName() const
{
	QString szName = ModelUtils::getUncapitalizedName(getName());
	return StringUtils::secureString(QString("_%0").arg(szName));
}

QString Element::getVariableNameList() const
{
	return getVariableName() + "List";
}

QString Element::getTagQualifiedName() const
{
	return m_pType->getTagQualifiedName();
}


QString Element::getSerializerDefinition(const QString& szClassname, const QString& szNamespace) const
{
	if(m_pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getVariableDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		return pComplexType->getSerializerDefinition(szClassname, szNamespace);
	}
}

QString Element::getDeserializerDefinition(const QString& szClassname) const
{
	if(m_pType->getTypeMode() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getDeserializerDefinition(szClassname);

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		return pComplexType->getDeserializerDefinition(szClassname);
	}
}

QString Element::getVariableDeclaration() const
{
	QString szDeclaration;

	QString szVariableName = getVariableName();

	if(m_pType->getTypeMode() == Type::TypeSimple)
	{
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);

		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			szDeclaration += "QList<";
			szDeclaration += pSimpleType->getCPPTypeNameString();
			szDeclaration += "> ";
			szDeclaration += szVariableName;
			szDeclaration += "List";
			szDeclaration += ";";
		} else {
			szDeclaration += pSimpleType->getCPPTypeNameString();
			szDeclaration += " ";
			szDeclaration += szVariableName;
			szDeclaration += ";";
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);

		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			szDeclaration += "QList<";
			szDeclaration += m_pType->getNameWithNamespace() + (m_bIsPointer ? "*" : "");
			szDeclaration += "> ";
			szDeclaration += szVariableName;
			szDeclaration += "List";
		} else {
			szDeclaration += (m_bIsNested || m_bIsPointer) ? m_pType->getLocalName() : m_pType->getNameWithNamespace();
			szDeclaration += (m_bIsNested || m_bIsPointer) ? "* " : " ";
			szDeclaration += szVariableName;
		}
		szDeclaration += ";";
	}

	return szDeclaration;
}

ElementList::ElementList()
{

}

ElementList::~ElementList()
{

}

ElementListSharedPtr ElementList::create()
{
	return ElementListSharedPtr(new ElementList());
}

ElementSharedPtr ElementList::getByRef(const QString& szRef) const
{
	const_iterator iter;
	for(iter = constBegin(); iter != constEnd(); ++iter){
		QString szNamespace = szRef.split(":")[0];
		QString szName = szRef.split(":")[1];

		if(((*iter)->getName() == szName) && ((*iter)->getNamespace() == szNamespace)){
			return *iter;
		}
	}
	return ElementSharedPtr();
}