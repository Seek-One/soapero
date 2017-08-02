/*
 * ComplexType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "SimpleType.h"
#include "ComplexType.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

Element::Element()
{
	m_iMinOccurs = 1; // -1 means unbounded
	m_iMaxOccurs = 1;
}

Element::~Element()
{

}

ElementSharedPtr Element::create()
{
	return ElementSharedPtr(new Element());
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
	return QString("_%0%1").arg(getName().left(1).toLower()).arg(getName().mid(1));
}

QString Element::getSetterDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getSetterDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDeclaration = "void set%0List(const QList<%1>& %2);" CRLF;
			szDeclaration += "\tvoid add%0(const %1& %3);";

			return szDeclaration.arg(szLocalName).arg(pComplexType->getNameWithNamespace()).arg(szVarListName).arg(szVarName);

		}else{
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szDeclaration = "void set%0(const %1& %2);";

			return szDeclaration.arg(getName()).arg(pComplexType->getNameWithNamespace()).arg(szVarName);
		}
	}
}

QString Element::getGetterDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getGetterDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szDeclaration = "QList<%0> get%1List() const;";
			return szDeclaration.arg(pComplexType->getNameWithNamespace()).arg(pComplexType->getLocalName());

		}else{
			QString szDeclaration = "%0 get%1() const;";
			return szDeclaration.arg(pComplexType->getNameWithNamespace()).arg(getName());
		}
	}
}

QString Element::getSerializerDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getSerializerDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {

			return "";

		} else {
			return pComplexType->getSerializerDeclaration();
		}
	}
}

QString Element::getSetterDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getSetterDefinition(szClassname);

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDeclaration = ""
			"void %0::set%1List(const QList<%2>& %3)" CRLF
			"{" CRLF
			"\t%4List = %3;" CRLF
			"}" CRLF CRLF
			"void %0::add%1(const %2& %5)" CRLF
			"{" CRLF
			"\t%4List.append(%5);" CRLF
			"}" CRLF;

			return szDeclaration.arg(szClassname).arg(szLocalName).arg(pComplexType->getNameWithNamespace())
					.arg(szVarListName).arg(pComplexType->getVariableName()).arg(szVarName);

		}else{
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);

			QString szDeclaration = ""
			"void %0::set%1(const %2& %3)" CRLF
			"{" CRLF
			"\t%4 = %3;" CRLF
			"}" CRLF;

			return szDeclaration.arg(szClassname).arg(getName()).arg(pComplexType->getNameWithNamespace())
					.arg(szVarName).arg(getVariableName());
		}
	}
}

QString Element::getGetterDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getGetterDefinition(szClassname);

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDefinition = ""
			"QList<%0> %1::get%2List() const" CRLF
			"{" CRLF
			"\treturn %3List;" CRLF
			"}" CRLF;

			return szDefinition.arg(pComplexType->getNameWithNamespace()).arg(szClassname).arg(pComplexType->getLocalName()).arg(pComplexType->getVariableName());

		}else{
			QString szDefinition = ""
			"%0 %1::get%2() const" CRLF
			"{" CRLF
			"\treturn %3;" CRLF
			"}" CRLF;

			return szDefinition.arg(pComplexType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(getVariableName());
		}
	}
}

QString Element::getSerializerDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getVariableDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		return pComplexType->getSerializerDefinition(szClassname);
	}
	/*AttributeList::const_iterator attr;
	ElementList::const_iterator elem;

	QString szDefinition = ""
	"QString %0::serialize() const" CRLF
	"{" CRLF
	"\treturn \"\"" CRLF
	"\t\"<%1>\"" CRLF;
	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());
			szDefinition += "\t\"\" + " + pSimpleType->getVariableName() + ".serialize() +" CRLF;

		}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());
			szDefinition += "\t\"\" + " + pComplexType->getVariableName() + ".serialize() + " CRLF;
		}

	}
	szDefinition += "\t\"</%1>\";" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(getTagQualifiedName());*/
}

QString Element::getVariableDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getVariableDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		QString szDeclaration;

		QString szVarName = "_" + getName().left(1).toLower() + getName().mid(1);

		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			szDeclaration += "QList<";
			szDeclaration += m_pType->getNameWithNamespace();
			szDeclaration += "> ";
			szDeclaration += szVarName;
			szDeclaration += "List";
		} else {
			szDeclaration += m_pType->getNameWithNamespace();
			szDeclaration += " ";
			szDeclaration += szVarName;
		}

		szDeclaration += ";";

		return szDeclaration;
	}
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

Attribute::Attribute()
{
	m_bRequired = false;
}

Attribute::~Attribute()
{

}

AttributeSharedPtr Attribute::create()
{
	return AttributeSharedPtr(new Attribute());
}

void Attribute::setName(const QString& szName)
{
	m_szName = szName;
}

QString Attribute::getName() const
{
	return m_szName;
}

void Attribute::setType(const TypeSharedPtr& pType)
{
	m_pType = pType;
}

TypeSharedPtr Attribute::getType() const
{
	return m_pType;
}

void Attribute::setRequired(bool bRequired)
{
	m_bRequired = bRequired;
}

bool Attribute::isRequired() const
{
	return m_bRequired;
}


AttributeList::AttributeList()
	:QList<AttributeSharedPtr>()
{

}

AttributeList::~AttributeList()
{

}

AttributeListSharedPtr AttributeList::create()
{
	return AttributeListSharedPtr(new AttributeList());
}

/*********************************************************
 *  COMPLEX TYPE
 */

ComplexType::ComplexType()
	:Type(Type::TypeComplex)
{
	m_pListAttribute = AttributeList::create();
	m_pListElement = ElementList::create();
}

ComplexType::~ComplexType()
{

}

ComplexTypeSharedPtr ComplexType::create()
{
	return ComplexTypeSharedPtr(new ComplexType());
}

void ComplexType::setExtensionType(TypeSharedPtr pType)
{
	m_pExtensionType = pType;
}

TypeSharedPtr ComplexType::getExtensionType() const
{
	return m_pExtensionType;
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

QString ComplexType::getSetterDeclaration() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration = "void set%0(const %1& %2);";
	return szDeclaration.arg(getLocalName()).arg(getNameWithNamespace()).arg(szVarName);
}

QString ComplexType::getGetterDeclaration() const
{
	QString szDeclaration = "%0 get%1() const;";
	return szDeclaration.arg(getNameWithNamespace()).arg(getLocalName());
}

QString ComplexType::getSerializerDeclaration() const
{
	return "QString serialize() const;";
}

QString ComplexType::getSetterDefinition(const QString& szClassname) const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration = ""
	"void %0::set%1(const %2& %3)" CRLF
	"{" CRLF
	"\t%4 = %5;" CRLF
	"}" CRLF;

	return szDeclaration.arg(szClassname).arg(getLocalName()).arg(getNameWithNamespace())
			.arg(szVarName).arg(getVariableName()).arg(szVarName);
}

QString ComplexType::getGetterDefinition(const QString& szClassname) const
{
	QString szDefinition = ""
	"%0 %1::get%2() const" CRLF
	"{" CRLF
	"\treturn %3;" CRLF
	"}" CRLF;

	return szDefinition.arg(getNameWithNamespace()).arg(szClassname).arg(getLocalName()).arg(getVariableName());
}

QString ComplexType::getSerializerDefinition(const QString& szClassname) const
{
	AttributeList::const_iterator attr;
	ElementList::const_iterator elem;

	QString szDefinition = ""
	"QString %0::serialize() const" CRLF
	"{" CRLF
	"\treturn \"\"" CRLF
	"\t\"<%1>\"" CRLF;
	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());
			szDefinition += "\t\"\" + " + pSimpleType->getVariableName() + ".serialize() +" CRLF;

		}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());
			szDefinition += "\t\"\" + " + pComplexType->getVariableName() + ".serialize() + " CRLF;
		}

	}
	szDefinition += "\t\"</%1>\";" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(getTagQualifiedName());
}

QString ComplexType::getVariableDeclaration() const
{
	QString szDeclaration;
	szDeclaration += getNameWithNamespace();
	szDeclaration += " ";
	szDeclaration += getVariableName();
	szDeclaration += ";";

	return szDeclaration;
}

QString ComplexType::getVariableName() const
{
	return "_" + getLocalName().left(1).toLower() + getLocalName().mid(1);
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

