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

#ifndef CRLF
#define CRLF "\r\n"
#endif

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

QString ComplexType::getSerializerDefinition(const QString& szClassname, const QString& szNamespace) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szIterator;
	int iIteratorCount = 0;

	QString szDefinition = ""
	"QString %0::serialize(bool bOnlyContent) const" CRLF
	"{" CRLF
	"\tQString szValue;" CRLF
	"\tif(!bOnlyContent) {" CRLF
	"\t\tszValue += \"<%1:%2\";" CRLF
	"\t}" CRLF;

	//Not found other cases so we supposed attribute is only a simpleType
	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if(pAttribute->getType()){
			if(pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
				if(pAttribute->isList()){
					szIterator = "iter_" + QString::number(iIteratorCount++);
					szDefinition += "\tif(_" + pAttribute->getName() + "List" + ".size() > 0) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\";" CRLF;
					szDefinition += QString("\t\tQList<%0>::const_iterator %1;" CRLF).arg( pSimpleType->getCPPTypeNameString()).arg(szIterator);
					szDefinition += QString("\t\tfor(%0 = _%1List.constBegin(); %0 != _%1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pAttribute->getName());
					szDefinition += QString("\t\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t\tszValue += %0->serialize() + \" \";" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t}" CRLF);
					szDefinition += "\t\t}" CRLF;
					szDefinition += "\t\tszValue +=\"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}else{
					szDefinition += "\tif(!" + pSimpleType->getVariableName() + ".isNull()) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\" + " + pSimpleType->getVariableName() + ".serialize() + \"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}
			}else if(pAttribute->getType()->getTypeMode() == Type::TypeComplex){
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());
				if(pAttribute->isList()){
					szIterator = "iter_" + QString::number(iIteratorCount++);
					szDefinition += "\tif(" + pAttribute->getName() + "List" + ".size() > 0) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\"" CRLF;
					szDefinition += QString("\t\tQList<%0>::const_iterator %1;" CRLF).arg( pComplexType->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\t\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pAttribute->getName());
					szDefinition += QString("\t\t\tif(!(*%0)->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t\tszValue += (*%0)->serialize() + \" \";" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\t}" CRLF);
					szDefinition += "\t\t}" CRLF;
					szDefinition += "\t\tszValue +=\"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}else{
					QString szVariableName = "_" + ModelUtils::getUncapitalizedName(pAttribute->getName());
					szDefinition += "\tif(!" + szVariableName + ".isNull()) {" CRLF;
					szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\" + " + szVariableName + ".serialize() + \"\\\"\";" CRLF;
					szDefinition += "\t}" CRLF;
				}
			}
		}
	}

	if(!getExtensionType().isNull()) {
		QString szExtensionName;
		if(getExtensionType()->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = getExtensionType()->getNameWithNamespace();
		}

		if(isExtensionTypeList()){
			szIterator = "iter_" + QString::number(iIteratorCount++);
			szDefinition += "\tQList<" + szExtensionName + ">::const_iterator " + szIterator + " = constBegin();" CRLF;
			szDefinition += "\twhile(" + szIterator + " != constEnd()){" CRLF;
			szDefinition += "\t\tszValue += (*" + szIterator + ").serialize(" +
					QString(((getExtensionType()->getTypeMode() == Type::TypeSimple) ? "" : "true")) +
					") + \" \";" CRLF;
			szDefinition += "\t\t++" + szIterator + ";" CRLF;
			szDefinition += "\t}" CRLF;
		}else{
			szDefinition += "\tszValue += " +
					szExtensionName +
					"::serialize(" +
					((getExtensionType()->getTypeMode() == Type::TypeSimple) ? "" : "true") +
					");" CRLF;
		}
	}else{
		szDefinition += "\t\tszValue += \">\";" CRLF;
	}

	for(elem = m_pListElement->constBegin(); elem != m_pListElement->constEnd(); ++elem) {
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szIterator = "iter_" + QString::number(iIteratorCount++);
				if(pSimpleType->isEnumeration()){
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getNameWithNamespace()).arg(szIterator);
				}else{
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getCPPTypeNameString()).arg(szIterator);
				}
				szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
				szDefinition += QString("\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
				szDefinition += QString("\t\t\tszValue += \"<" + szNamespace + ":" + pElement->getName() + ">\" + %0->serialize() + \"</" + szNamespace + ":" + pElement->getName() + ">\";" CRLF).arg(szIterator);
				szDefinition += QString("\t\t}" CRLF);
				szDefinition += "\t}" CRLF;
			} else{
				szDefinition += "\tif(!" + pElement->getVariableName() + ".isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + szNamespace + ":" + pElement->getName() + ">\" + " + pElement->getVariableName() + ".serialize() + \"</" + szNamespace + ":" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szIterator = "iter_" + QString::number(iIteratorCount++);
				if(pElement->isPointer()){
					szDefinition += QString("\tQList<%0*>::const_iterator %1;" CRLF).arg( pComplexType->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
					szDefinition += QString("\t\tif(*%0 && !(*%0)->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += (*%0)->serialize();" CRLF).arg(szIterator);
					szDefinition += QString("\t\t}" CRLF);
					szDefinition += "\t}" CRLF;
				}else{
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg( pComplexType->getNameWithNamespace()).arg(szIterator);
					szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
					szDefinition += QString("\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += \"<%0:%1\";" CRLF).arg(szNamespace).arg(pElement->getName());
					szDefinition += QString("\t\t\tszValue += %0->serialize(true);" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += \"</%0:%1>\";" CRLF).arg(szNamespace).arg(pElement->getName());
					szDefinition += QString("\t\t}" CRLF);
					szDefinition += "\t}" CRLF;
				}
			} else{
				szDefinition += "\tif(" + ((pElement->isNested() || pElement->isPointer()) ? pElement->getVariableName() + " && " : "") +
						"!" + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + szNamespace + ":" + pElement->getName() + "\" + " + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "serialize(true) + \"</" + szNamespace + ":" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}
	szDefinition += "\tif(!bOnlyContent) {" CRLF;
	szDefinition += "\t\tszValue += \"</%1:%2>\";" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += "\treturn szValue;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(szNamespace).arg(getLocalName());
}

QString ComplexType::getDeserializerDefinition(const QString& szClassname) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;
	AttributeSharedPtr pAttribute;
	ElementSharedPtr pElement;

	QString szDefinition = ""
	"void %0::deserialize(QDomElement& element)" CRLF
	"{" CRLF;

	if(!getExtensionType().isNull()) {
		QString szExtensionName;
		if(getExtensionType()->getTypeMode() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getCPPTypeNameString();
		}else{
			szExtensionName = getExtensionType()->getNameWithNamespace();
		}

		if(isExtensionTypeList()){
			szDefinition += "\tQString szElement = element.text();" CRLF;
			szDefinition += "\tQStringList listElements = szElement.split(\" \");" CRLF;
			szDefinition += "\tfor(int i = 0; i < listElements.size(); ++i){" CRLF;
			szDefinition += "\t\tQDomAttr attr;" CRLF;
			szDefinition += "\t\tattr.setValue(listElements[i]);" CRLF;
			szDefinition += "\t\t" + szExtensionName + " item;" CRLF;
			szDefinition += "\t\titem.deserialize(attr);" CRLF;
			szDefinition += "\t\tappend(item);" CRLF;
			szDefinition += "\t}" CRLF;
		}else{
			szDefinition += "\t" + szExtensionName + "::deserialize(element);" CRLF;
		}
	}

	//Not found other cases so we supposed attribute is only a simpleType
	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if( pAttribute->getType() && pAttribute->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
			if(pAttribute->isList()){
				szDefinition += "\tif(!element.attributeNode(\"" + pAttribute->getName() + "\").isNull()) {" CRLF;
				szDefinition += "\t\tQDomAttr attribute = element.attributeNode(\"" + pAttribute->getName() + "\");" CRLF;
				szDefinition += "\t\tQString szAttrValue = attribute.value();" CRLF;
				szDefinition += "\t\tif(!szAttrValue.isEmpty()){" CRLF;
				szDefinition += "\t\t\tQStringList szValues = szAttrValue.split(\" \");" CRLF;
				szDefinition += "\t\t\tfor(int i = 0; i < szValues.size(); ++i){" CRLF;
				szDefinition += "\t\t\t\tif(!szValues[i].isEmpty()){" CRLF;
				szDefinition += "\t\t\t\t\t" + pSimpleType->getCPPTypeNameString() + " tmp;" CRLF;
				szDefinition += "\t\t\t\t\ttmp.setValue(szValues[i]);" CRLF;
				szDefinition += "\t\t\t\t\tadd" + ModelUtils::getCapitalizedName(pAttribute->getName()) + "(tmp);" CRLF;
				szDefinition += "\t\t\t\t}" CRLF;
				szDefinition += "\t\t\t}" CRLF;
				szDefinition += "\t\t}" CRLF;
				szDefinition += "\t}" CRLF;
			}else{
				szDefinition += "\tif(!element.attributeNode(\"" + pAttribute->getName() + "\").isNull()) {" CRLF;
				szDefinition += "\t\t" + pSimpleType->getVariableName() + ".deserialize(element.attributeNode(\"" + pAttribute->getName() + "\"));" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}
	szDefinition += CRLF;
	szDefinition += "\tQDomElement child = element.firstChild().toElement();" CRLF;
	szDefinition += "\twhile(!child.isNull()) {" CRLF;
	for(elem = m_pListElement->constBegin(); elem != m_pListElement->constEnd(); ++elem) {
		if((*elem)->hasRef()){
			pElement = (*elem)->getRef();
		}else{
			pElement = *elem;
		}

		if(!pElement->getType()) {
			continue;
		}

		if(pElement->getType()->getTypeMode() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szDefinition += "\t\tif(child.tagName() == \"" + getNamespace() + ":" + pElement->getName() + "\") {" CRLF;
				if(pSimpleType->isEnumeration()){
					szDefinition += "\t\t\t" + pSimpleType->getNameWithNamespace() + " item;" CRLF;
				}else{
					szDefinition += "\t\t\t" + pSimpleType->getCPPTypeNameString() + " item;" CRLF;
				}
				szDefinition += "\t\t\titem.deserialize(child);" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
				szDefinition += "\t\t}" CRLF;

			} else{
				szDefinition += "\t\tif((child.tagName() == \"" + getNamespace() + ":" + pElement->getName() + "\")" +
						" || (child.tagName().endsWith(\":" + pElement->getName() + "\"" + "))"
						" || (child.tagName() == \"" + pElement->getName() + "\"" + ")){" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + ".deserialize(child);" CRLF;
				szDefinition += "\t\t}" CRLF;
			}

		}else if(pElement->getType()->getTypeMode() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				if(pElement->isPointer()){
					szDefinition += "\t\tif(child.tagName() == \"" + pComplexType->getNamespace() + ":" + pElement->getName() + "\") {" CRLF;
					szDefinition += "\t\t\t" + pComplexType->getNameWithNamespace() + "* item = new " + pComplexType->getNameWithNamespace() + "();" CRLF;
					szDefinition += "\t\t\titem->deserialize(child);" CRLF;
					szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
					szDefinition += "\t\t}" CRLF;
				}else{
					szDefinition += "\t\tif((child.tagName() == \"" + pComplexType->getNamespace() + ":" + pElement->getName() + "\") " +
							" || child.tagName().endsWith(\":" + pElement->getName() + "\"" + ")){" CRLF;
					szDefinition += "\t\t\t" + pComplexType->getNameWithNamespace() + " item;" CRLF;
					szDefinition += "\t\t\titem.deserialize(child);" CRLF;
					szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
					szDefinition += "\t\t}" CRLF;
				}
			} else{
				szDefinition += "\t\tif((child.tagName() == \"" + pComplexType->getNamespace() + ":" + pElement->getName() + "\") " +
						" || child.tagName().endsWith(\":" + pElement->getName() + "\"" + ")){" CRLF;
				if((pElement->isNested() || pElement->isPointer())){
					szDefinition += "\t\t\tif(!" + pElement->getVariableName() + "){" CRLF;
					szDefinition += "\t\t\t\t" + pElement->getVariableName() + " = new " + pElement->getType()->getLocalName() + "();" CRLF;
					szDefinition += "\t\t\t}" CRLF;
				}
				szDefinition += "\t\t\t" + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "deserialize(child);" CRLF;
				szDefinition += "\t\t}" CRLF;
			}
		}
	}
	szDefinition += "\t\tchild = child.nextSibling().toElement();" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname);
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

QString ComplexType::getVariableDeclaration(const QString& szName) const
{
	QString szVarName;
	QString szDeclaration;
	if(!szName.isEmpty()){
		szVarName = "_" + ModelUtils::getUncapitalizedName(szName);
	}else{
		szVarName = getVariableName();
	}
	szDeclaration += getNameWithNamespace();
	szDeclaration += " ";
	szDeclaration += szVarName;
	szDeclaration += ";";

	return szDeclaration;
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
