/*
 * ComplexType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "../Utils/StringUtils.h"
#include "../Utils/ModelUtils.h"

#include "SimpleType.h"
#include "Element.h"
#include "Attribute.h"

#include "ComplexType.h"

#include "../common.h"

ComplexType::ComplexType() : Type(Type::CategoryType, Type::TypeComplex)
{
	m_pListAttribute = AttributeList::create();
	m_pListElement = ElementList::create();

	m_bIsListExtension = false;

	m_bIsSoapEnvelopeFault = false;
}

ComplexType::ComplexType(const ComplexType& other)
	: Type(other)
{
	m_pListAttribute = other.m_pListAttribute;
	m_pListElement = other.m_pListElement;

	m_pExtensionType = other.m_pExtensionType;
	m_bIsListExtension = other.m_bIsListExtension;

	m_bIsSoapEnvelopeFault = other.m_bIsSoapEnvelopeFault;
}

ComplexType::~ComplexType()
{

}

ComplexTypeSharedPtr ComplexType::create()
{
	return ComplexTypeSharedPtr(new ComplexType());
}

ComplexTypeSharedPtr ComplexType::clone()
{
	return ComplexTypeSharedPtr(new ComplexType(*this));
}

void ComplexType::setExtensionType(TypeSharedPtr pType, bool bIsList)
{
	m_pExtensionType = pType;
	m_bIsListExtension = bIsList;
}

TypeSharedPtr ComplexType::getExtensionType() const
{
	return m_pExtensionType;
}

bool ComplexType::isExtensionTypeList() const
{
	return m_bIsListExtension;
}

void ComplexType::setIsSoapEnvelopeFault(bool bIsSoapEnvelopeFault)
{
	m_bIsSoapEnvelopeFault = bIsSoapEnvelopeFault;
}

bool ComplexType::isSoapEnvelopeFault() const
{
	return m_bIsSoapEnvelopeFault;
}

void ComplexType::addAttribute(const AttributeSharedPtr& pAttribute)
{
	m_pListAttribute->append(pAttribute);
}

AttributeListSharedPtr ComplexType::getAttributeList() const
{
	return m_pListAttribute;
}

void ComplexType::addElement(const ElementSharedPtr& pElement)
{
	m_pListElement->append(pElement);
}

ElementListSharedPtr ComplexType::getElementList() const
{
	return m_pListElement;
}


QString ComplexType::getIsNullDefinition(const QString& szClassname) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition = ""
	"bool %0::isNull() const" CRLF
	"{" CRLF
	"\treturn ";
	QString szCond = "";

	if(!getExtensionType().isNull()) {
		QString szExtensionName;
		if(getExtensionType()->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = getExtensionType()->getNameWithNamespace();
		}

		if(isExtensionTypeList()){
			szCond += "isEmpty() && ";
		}else{
			szCond += szExtensionName + "::isNull() && ";
		}
	}

	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if(pAttribute->getType()){
			if( pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
				if(pAttribute->isList()){
					szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
				}else{
					szCond += pSimpleType->getVariableName() + ".isNull() && ";
				}

			}else if( pAttribute->getType()->getTypeMode() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());
				if(pAttribute->isList()){
					szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
				}else{
					QString szVariableName = "_" + ModelUtils::getUncapitalizedName(pAttribute->getName());
					szCond += szVariableName + ".isNull() && ";
				}
			}
		}
	}
	for(elem = m_pListElement->constBegin(); elem != m_pListElement->constEnd(); ++elem) {
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if( !pElement->getType()) {
			continue;
		}
		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());
			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szCond += pElement->getVariableName() + "List.isEmpty() && ";
			}else{
				szCond += pElement->getVariableName() + ".isNull() && ";
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szCond += pElement->getVariableName() + "List.isEmpty() && ";
			}else{
				if((pElement->isNested() || pElement->isPointer())){
					szCond += "(!" + pElement->getVariableName() + " || (" + pElement->getVariableName() + " && " + pElement->getVariableName() + "->isNull())) && ";
				}else{
					szCond += pElement->getVariableName() + ".isNull() && ";
				}
			}
		}
	}
	if(szCond.length() > 1) {
		szDefinition += szCond.left(szCond.length() - 4);
		szDefinition += ";" CRLF;
	}else{
		szDefinition += "true;" CRLF;
	}
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname);
}

QString ComplexType::getGetNamespaceDeclarationDefinition(const QString& szClassname) const
{
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition = ""
	"QList<QString> %0::getNamespaceDeclaration()" CRLF
	"{" CRLF;
	szDefinition += "\tQList<QString> listNamespaceDeclaration;" CRLF;

	if(!getExtensionType().isNull()){
		QString szExtensionName;
		if(getExtensionType()->getTypeMode() == Type::TypeComplex){
			szDefinition += "\tlistNamespaceDeclaration.append(" + getExtensionType()->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;
		}
	}

	for(elem = m_pListElement->constBegin(); elem != m_pListElement->constEnd(); ++elem){
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if((pElement->getType()->getTypeMode() == Type::TypeComplex) &&
				(getLocalName(true) != pElement->getType()->getLocalName(true))){
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
			szDefinition += "\tlistNamespaceDeclaration.append(" + pComplexType->getNameWithNamespace() + "::getNamespaceDeclaration());" CRLF;
		}
	}

	szDefinition += "\tlistNamespaceDeclaration.append(\"xmlns:" + getNamespace() + "=\\\"" + getNamespaceUri() + "\\\"\");" CRLF;
	szDefinition += "\treturn listNamespaceDeclaration;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname);
}

QString ComplexType::getIsNullDeclaration() const
{
	return "bool isNull() const;";
}

QString ComplexType::getGetNamespaceDeclarationDeclaration() const
{
	return "static QList<QString> getNamespaceDeclaration();";
}

QString ComplexType::getVariableName() const
{
	return "_" + ModelUtils::getUncapitalizedName(getLocalName());
}

ComplexTypeList::ComplexTypeList()
	:QList<ComplexTypeSharedPtr>()
{

}

ComplexTypeList::~ComplexTypeList()
{

}

ComplexTypeListSharedPtr ComplexTypeList::create()
{
	return ComplexTypeListSharedPtr(new ComplexTypeList());
}
