/*
 * ComplexType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "../Utils/StringUtils.h"
#include "SimpleType.h"
#include "ComplexType.h"

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
	return StringUtils::secureString(QString("_%0%1").arg(getName().left(1).toLower()).arg(getName().mid(1)));
}

QString Element::getTagQualifiedName() const
{
	return m_pType->getTagQualifiedName();
}

QString Element::getSetterDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDeclaration = "void set%0List(const QList<%1>& %2);" CRLF;
			szDeclaration += "\tvoid add%0(const %1& %3);";

			if(pSimpleType->isEnumeration()){
				return szDeclaration.arg(szLocalName).arg(pSimpleType->getNameWithNamespace()).arg(szVarListName).arg(szVarName);
			}else{
				return szDeclaration.arg(szLocalName).arg(pSimpleType->getVariableTypeString()).arg(szVarListName).arg(szVarName);
			}
		}else{
			if(pSimpleType->isEnumeration()) {
				QString szVarName = getName().left(1).toLower() + getName().mid(1);
				QString szDeclaration = "void set%0(const %1& %2);";

				return szDeclaration.arg(getName()).arg(pSimpleType->getNameWithNamespace()).arg(szVarName);

			} else {
				QString szVarName = StringUtils::secureString(getName().left(1).toLower() + getName().mid(1));
				QString szDeclaration = "void set%0(const %1& %2);";

				return szDeclaration.arg(StringUtils::secureString(getName())).arg(pSimpleType->getVariableTypeString()).arg(szVarName);
			}
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDeclaration;
			if(m_bIsPointer){
				szDeclaration = "void set%0List(const QList<%1*>& %2);" CRLF;
				szDeclaration += "\tvoid add%0(%1* %3);";
			}else{
				szDeclaration = "void set%0List(const QList<%1>& %2);" CRLF;
				szDeclaration += "\tvoid add%0(const %1& %3);";
			}

			return szDeclaration.arg(szLocalName).arg(pComplexType->getNameWithNamespace()).arg(szVarListName).arg(szVarName);

		}else{
			QString szLocalName = pComplexType->getLocalName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szDeclaration;
			if(m_bIsNested || m_bIsPointer){
				szDeclaration = QString("void set%0(%1* %2);").arg(getName()).arg(pComplexType->getLocalName()).arg(szVarName);
			}else{
				szDeclaration = QString("void set%0(const %1& %2);").arg(getName()).arg(pComplexType->getNameWithNamespace()).arg(szVarName);
			}

			return szDeclaration;
		}
	}
}

QString Element::getGetterDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szDeclaration = "const QList<%0>& get%1List() const;";
			if(pSimpleType->isEnumeration()){
				return szDeclaration.arg(pSimpleType->getNameWithNamespace()).arg(getName());
			}else{
				return szDeclaration.arg(pSimpleType->getVariableTypeString()).arg(getName());
			}

		}else{
			if(pSimpleType->isEnumeration()) {
				QString szDeclaration = "%0 get%1() const;";
				return szDeclaration.arg(pSimpleType->getNameWithNamespace()).arg(getName());
			} else {
				QString szDeclaration = "const %0& get%1() const;";
				return szDeclaration.arg(pSimpleType->getVariableTypeString()).arg(StringUtils::secureString(getName()));
			}
		}

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szDeclaration;
			if(m_bIsPointer){
				szDeclaration = "const QList<%0*>& get%1List() const;";
			}else{
				szDeclaration = "const QList<%0>& get%1List() const;";
			}

			return szDeclaration.arg(pComplexType->getNameWithNamespace()).arg(getName());

		}else{
			QString szDeclaration;
			if(m_bIsNested || m_bIsPointer){
				szDeclaration = QString("%0* get%1() const;").arg(pComplexType->getLocalName()).arg(getName());
			}else{
				szDeclaration = QString("const %0& get%1() const;").arg(pComplexType->getNameWithNamespace()).arg(getName());
			}
			return szDeclaration;
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

QString Element::getDeserializerDeclaration() const
{
	return "void deserialize(QDomElement& element);";
}

QString Element::getSetterDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
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

			if(pSimpleType->isEnumeration()){
				return szDeclaration.arg(szClassname).arg(szLocalName).arg(pSimpleType->getNameWithNamespace())
										.arg(szVarListName).arg(getVariableName()).arg(szVarName);
			}else{
				return szDeclaration.arg(szClassname).arg(szLocalName).arg(pSimpleType->getVariableTypeString())
						.arg(szVarListName).arg(getVariableName()).arg(szVarName);
			}

		}else{
			if(!pSimpleType->isEnumeration()) {
				QString szVarName = StringUtils::secureString(getName().left(1).toLower() + getName().mid(1));

				QString szDefinition = "";
				szDefinition = "void %0::set%1(const %2& %3)" CRLF;
				szDefinition += "{" CRLF;
				szDefinition += "\t%4 = %5;" CRLF;
				szDefinition += "}" CRLF;

				return szDefinition.arg(szClassname).arg(StringUtils::secureString(getName())).arg(pSimpleType->getVariableTypeString())
									.arg(szVarName).arg(getVariableName()).arg(szVarName);
			} else {
				QString szLocalName = StringUtils::secureString(getName());
				QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);

				QString szDefinition = "void %0::set%1(const %2& %3)" CRLF;
				szDefinition += "{" CRLF;
				szDefinition += "\t%4 = %3;" CRLF;
				szDefinition += "}" CRLF;

				return szDefinition.arg(szClassname).arg(szLocalName).arg(pSimpleType->getNameWithNamespace())
									.arg(szVarName).arg(getVariableName());
			}
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDeclaration;
			if(m_bIsPointer){
				szDeclaration = ""
						"void %0::set%1List(const QList<%2*>& %3)" CRLF
						"{" CRLF
						"\t%4List = %3;" CRLF
						"}" CRLF CRLF
						"void %0::add%1(%2* %5)" CRLF
						"{" CRLF
						"\t%4List.append(%5);" CRLF
						"}" CRLF;
			}else{
				szDeclaration = ""
						"void %0::set%1List(const QList<%2>& %3)" CRLF
						"{" CRLF
						"\t%4List = %3;" CRLF
						"}" CRLF CRLF
						"void %0::add%1(const %2& %5)" CRLF
						"{" CRLF
						"\t%4List.append(%5);" CRLF
						"}" CRLF;
			}


			return szDeclaration.arg(szClassname).arg(szLocalName).arg(pComplexType->getNameWithNamespace())
					.arg(szVarListName).arg(getVariableName()).arg(szVarName);

		}else{
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szDeclaration;
			if(m_bIsNested || m_bIsPointer){
				szDeclaration = QString("void %0::set%1(%2* %3)" CRLF
						"{" CRLF
						"\t%4 = %3;" CRLF
						"}" CRLF).arg(szClassname).arg(szLocalName).arg(pComplexType->getLocalName()).arg(szVarName).arg(getVariableName());
			}else{
				szDeclaration = QString("void %0::set%1(const %2& %3)" CRLF
						"{" CRLF
						"\t%4 = %3;" CRLF
						"}" CRLF).arg(szClassname).arg(szLocalName).arg(pComplexType->getNameWithNamespace()).arg(szVarName).arg(getVariableName());
			}

			return szDeclaration;
		}
	}
}

QString Element::getGetterDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDefinition = ""
			"const QList<%0>& %1::get%2List() const" CRLF
			"{" CRLF
			"\treturn %3List;" CRLF
			"}" CRLF;

			if(pSimpleType->isEnumeration()){
				return szDefinition.arg(pSimpleType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(getVariableName());
			}else{
				return szDefinition.arg(pSimpleType->getVariableTypeString()).arg(szClassname).arg(getName()).arg(getVariableName());
			}
		}else{
			if(pSimpleType->isEnumeration()) {
				QString szDefinition = ""
				"%0 %1::get%2() const" CRLF
				"{" CRLF
				"\treturn %3;" CRLF
				"}" CRLF;

				return szDefinition.arg(pSimpleType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(getVariableName());
			}else{
				QString szDefinition = ""
				"const %0& %1::get%2() const" CRLF
				"{" CRLF
				"\treturn %3;" CRLF
				"}" CRLF;

				return szDefinition.arg(pSimpleType->getVariableTypeString()).arg(szClassname).arg(StringUtils::secureString(getName())).arg(getVariableName());
			}
		}

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			QString szDefinition;
			if(m_bIsPointer){
				szDefinition = ""
						"const QList<%0*>& %1::get%2List() const" CRLF
						"{" CRLF
						"\treturn %3List;" CRLF
						"}" CRLF;
			}else{
				szDefinition = ""
						"const QList<%0>& %1::get%2List() const" CRLF
						"{" CRLF
						"\treturn %3List;" CRLF
						"}" CRLF;
			}

			return szDefinition.arg(pComplexType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(getVariableName());

		}else{
			QString szDefinition;
			if(m_bIsNested || m_bIsPointer){
				szDefinition = QString("%0* %1::get%2() const" CRLF
						"{" CRLF
						"\treturn %3;" CRLF
						"}" CRLF).arg(pComplexType->getLocalName()).arg(szClassname).arg(getName()).arg(getVariableName());
			}else{
				szDefinition = QString("const %0& %1::get%2() const" CRLF
						"{" CRLF
						"\treturn %3;" CRLF
						"}" CRLF).arg(pComplexType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(getVariableName());
			}

			return szDefinition;
		}
	}
}

QString Element::getSerializerDefinition(const QString& szClassname, const QString& szNamespace) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getVariableDeclaration();

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		return pComplexType->getSerializerDefinition(szClassname, szNamespace);
	}
}

QString Element::getDeserializerDefinition(const QString& szClassname) const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		return pSimpleType->getDeserializerDefinition(szClassname);

	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		return pComplexType->getDeserializerDefinition(szClassname);
	}
}

QString Element::getVariableDeclaration() const
{
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		QString szDeclaration;

		QString szVarName = StringUtils::secureString("_" + getName().left(1).toLower() + getName().mid(1));

		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			szDeclaration += "QList<";
			if(pSimpleType->isEnumeration()){
				szDeclaration += pSimpleType->getNameWithNamespace();
			}else{
				szDeclaration += pSimpleType->getVariableTypeString();
			}
			szDeclaration += "> ";
			szDeclaration += szVarName;
			szDeclaration += "List";
			szDeclaration += ";";

			return szDeclaration;

		} else {
			QString szDeclaration;
			if(!pSimpleType->isEnumeration()) {
				szDeclaration += pSimpleType->getVariableTypeString();
			} else {
				szDeclaration += pSimpleType->getNameWithNamespace();
			}
			szDeclaration += " ";
			szDeclaration += szVarName;
			szDeclaration += ";";

			return szDeclaration;
		}
	}else{
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		QString szDeclaration;

		QString szVarName = "_" + getName().left(1).toLower() + getName().mid(1);

		if(m_iMaxOccurs > 1 || m_iMaxOccurs == -1) {
			szDeclaration += "QList<";
			szDeclaration += m_pType->getNameWithNamespace() + (m_bIsPointer ? "*" : "");
			szDeclaration += "> ";
			szDeclaration += szVarName;
			szDeclaration += "List";
		} else {
			szDeclaration += (m_bIsNested || m_bIsPointer) ? m_pType->getLocalName() : m_pType->getNameWithNamespace();
			szDeclaration += (m_bIsNested || m_bIsPointer) ? "* " : " ";
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

Attribute::Attribute()
{
	m_bRequired = false;
	m_bIsList = false;
}

Attribute::~Attribute()
{

}

AttributeSharedPtr Attribute::create()
{
	return AttributeSharedPtr(new Attribute());
}

void Attribute::setRef(const AttributeSharedPtr& pRef)
{
	m_pRef = pRef;
}

const AttributeSharedPtr& Attribute::getRef() const
{
	return m_pRef;
}

bool Attribute::hasRef() const
{
	return !m_pRef.isNull();
}

void Attribute::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

const QString& Attribute::getNamespace() const
{
	return m_szNamespace;
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

void Attribute::setIsList(bool bIsList)
{
	m_bIsList = bIsList;
}

bool Attribute::isList() const
{
	return m_bIsList;
}

QString Attribute::getSetterDeclaration() const
{
	QString szDeclaration;
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);

		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDeclaration = "void set%0List(const QList<%1>& %2);" CRLF;
			szDeclaration += "\tvoid add%0(const %1& %3);" CRLF;
			szDeclaration = szDeclaration.arg(szLocalName).arg(pSimpleType->getVariableTypeString()).arg(szVarListName).arg(szVarName);
		}else if(pSimpleType->isEnumeration()) {
			szDeclaration += pSimpleType->getSetterDeclarationForComplexType() + CRLF;
		} else {
			szDeclaration += pSimpleType->getSetterDeclaration() + CRLF;
		}

	}else if(m_pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);

		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDeclaration = "void set%0List(const QList<%1>& %2);" CRLF;
			szDeclaration += "\tvoid add%0(const %1& %3);" CRLF;
			szDeclaration = szDeclaration.arg(szLocalName).arg(pComplexType->getNameWithNamespace()).arg(szVarListName).arg(szVarName);
		}else{
			szDeclaration += pComplexType->getSetterDeclaration(getName()) + CRLF;
		}
	}
	return szDeclaration;
}

QString Attribute::getGetterDeclaration() const
{
	QString szDeclaration;
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);

		if(m_bIsList){
			szDeclaration = "const QList<%0>& get%1List() const;" CRLF;
			szDeclaration = szDeclaration.arg(pSimpleType->getVariableTypeString()).arg(getName());
		}else if(pSimpleType->isEnumeration()) {
			szDeclaration += pSimpleType->getGetterDeclarationForComplexType() + CRLF;
		} else {
			szDeclaration += pSimpleType->getGetterDeclaration() + CRLF;
		}

	}else if(m_pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_bIsList){
			szDeclaration = "const QList<%0>& get%1List() const;" CRLF;
			szDeclaration = szDeclaration.arg(pComplexType->getNameWithNamespace()).arg(getName());
		}else{
			szDeclaration += pComplexType->getGetterDeclaration(getName()) + CRLF ;
		}
	}
	return szDeclaration;
}

QString Attribute::getVariableDeclaration() const
{
	QString szDeclaration;
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);
		if(m_bIsList){
			szDeclaration += "QList<";
			szDeclaration += pSimpleType->getVariableTypeString();
			szDeclaration += "> ";
			szDeclaration += "_" + getName().left(1).toLower() + getName().mid(1);
			szDeclaration += "List";
			szDeclaration += ";";
		}else{
			szDeclaration += pSimpleType->getVariableDeclarationForComplexType() ;
		}
	}else if(m_pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);if(m_bIsList){
			szDeclaration += "QList<";
			szDeclaration += pComplexType->getNameWithNamespace();
			szDeclaration += "> ";
			szDeclaration += "_" + getName().left(1).toLower() + getName().mid(1);
			szDeclaration += "List";
			szDeclaration += ";";
		}else{
			szDeclaration += pComplexType->getVariableDeclaration(getName());
		}
	}
	return szDeclaration;
}

QString Attribute::getSetterDefinition(const QString& szClassname) const
{
	QString szDefinition;
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);

		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDefinition = "void %0::set%1List(const QList<%2>& %3)" CRLF
			"{" CRLF
			"\t_%4List = %3;" CRLF
			"}" CRLF CRLF
			"void %0::add%1(const %2& %5)" CRLF
			"{" CRLF
			"\t_%4List.append(%5);" CRLF
			"}" CRLF CRLF;

			szDefinition = szDefinition.arg(szClassname).arg(szLocalName).arg(pSimpleType->getVariableTypeString())
					.arg(szVarListName).arg(szLocalName).arg(szVarName);
		}else if(!pSimpleType->isEnumeration()) {
			szDefinition += pSimpleType->getSetterDefinition(szClassname) + CRLF;
		} else {
			szDefinition += pSimpleType->getSetterDefinitionForComplexType(szClassname) + CRLF;
		}

	}else if(m_pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);
		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDefinition = "void %0::set%1List(const QList<%2>& %3)" CRLF
			"{" CRLF
			"\t_%4List = %3;" CRLF
			"}" CRLF CRLF
			"void %0::add%1(const %2& %5)" CRLF
			"{" CRLF
			"\t_%4List.append(%5);" CRLF
			"}" CRLF CRLF;

			szDefinition = szDefinition.arg(szClassname).arg(szLocalName).arg(pComplexType->getNameWithNamespace())
					.arg(szVarListName).arg(szLocalName).arg(szVarName);
		}else{
			szDefinition += pComplexType->getSetterDefinition(szClassname, getName()) + CRLF;
		}
	}
	return szDefinition;
}

QString Attribute::getGetterDefinition(const QString& szClassname) const
{
	QString szDefinition;
	if(m_pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(m_pType);

		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDefinition = ""
			"const QList<%0>& %1::get%2List() const" CRLF
			"{" CRLF
			"\treturn _%3List;" CRLF
			"}" CRLF;

			szDefinition = szDefinition.arg(pSimpleType->getVariableTypeString()).arg(szClassname).arg(getName()).arg(szVarName);
		}else if(!pSimpleType->isEnumeration()) {
			szDefinition += pSimpleType->getGetterDefinition(szClassname);
		} else {
			szDefinition += pSimpleType->getGetterDefinitionForComplexType(szClassname);
		}

	}else if(m_pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(m_pType);

		if(m_bIsList){
			QString szLocalName = getName();
			QString szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);
			QString szVarListName = szVarName + "List";
			szDefinition = ""
			"const QList<%0>& %1::get%2List() const" CRLF
			"{" CRLF
			"\treturn _%3List;" CRLF
			"}" CRLF;

			szDefinition = szDefinition.arg(pComplexType->getNameWithNamespace()).arg(szClassname).arg(getName()).arg(szVarName);
		}else{
			szDefinition += pComplexType->getGetterDefinition(szClassname, getName());
		}
	}
	return szDefinition;
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

AttributeSharedPtr AttributeList::getByRef(const QString& szRef) const
{
	const_iterator iter;
	for(iter = constBegin(); iter != constEnd(); ++iter){
		QString szNamespace = szRef.split(":")[0];
		QString szName = szRef.split(":")[1];

		if(((*iter)->getName() == szName) && ((*iter)->getNamespace() == szNamespace)){
			return *iter;
		}
	}
	return AttributeSharedPtr();
}

bool AttributeList::containsListAttribute() const
{
	const_iterator iter;
	for(iter = constBegin(); iter != constEnd(); ++iter){
		if((*iter)->isList()){
			return true;
		}
	}
	return false;
}

/*********************************************************
 *  COMPLEX TYPE
 */

ComplexType::ComplexType()
	:Type(Type::TypeComplex)
{
	m_pListAttribute = AttributeList::create();
	m_pListElement = ElementList::create();

	m_bIsListExtension = false;

	m_bIsSoapEnvelopeFault = false;
}

ComplexType::~ComplexType()
{

}

ComplexTypeSharedPtr ComplexType::create()
{
	return ComplexTypeSharedPtr(new ComplexType());
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

QString ComplexType::getSetterDeclaration(const QString& szName) const
{
	QString szLocalName;
	QString szVarName;
	if(!szName.isEmpty()){
		szLocalName = szName;
	}else{
		szLocalName = getLocalName();
	}
	szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);

	QString szDeclaration = "void set%0(const %1& %2);";
	return szDeclaration.arg(szLocalName).arg(getNameWithNamespace()).arg(szVarName);
}

QString ComplexType::getGetterDeclaration(const QString& szName) const
{
	QString szLocalName;
	if(!szName.isEmpty()){
		szLocalName = szName;
	}else{
		szLocalName = getLocalName();
	}
	QString szDeclaration = "const %0& get%1() const;";
	return szDeclaration.arg(getNameWithNamespace()).arg(szLocalName);
}

QString ComplexType::getSerializerDeclaration() const
{
	return "QString serialize(bool bOnlyContent = false) const;";
}

QString ComplexType::getDeserializerDeclaration() const
{
	return "void deserialize(QDomElement& element);";
}

QString ComplexType::getSetterDefinition(const QString& szClassname, const QString& szName) const
{
	QString szLocalName;
	QString szVarName;
	if(!szName.isEmpty()){
		szLocalName = szName;
	}else{
		szLocalName = getLocalName();
	}
	szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);

	QString szDeclaration = ""
	"void %0::set%1(const %2& %3)" CRLF
	"{" CRLF
	"\t%4 = %5;" CRLF
	"}" CRLF;

	return szDeclaration.arg(szClassname).arg(szLocalName).arg(getNameWithNamespace())
			.arg(szVarName).arg("_" + szVarName).arg(szVarName);
}

QString ComplexType::getGetterDefinition(const QString& szClassname, const QString& szName) const
{
	QString szLocalName;
	QString szVarName;
	if(!szName.isEmpty()){
		szLocalName = szName;
	}else{
		szLocalName = getLocalName();
	}
	szVarName = szLocalName.left(1).toLower() + szLocalName.mid(1);

	QString szDefinition = ""
	"const %0& %1::get%2() const" CRLF
	"{" CRLF
	"\treturn %3;" CRLF
	"}" CRLF;

	return szDefinition.arg(getNameWithNamespace()).arg(szClassname).arg(szLocalName).arg("_" + szVarName);
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
	"\tif(!bOnlyContent) {" CRLF;
	if(szNamespace.isNull()) {
		szDefinition += "\t\tszValue += \"<%1\";" CRLF;
	}else{
		szDefinition += "\t\tszValue += \"<%1 xmlns=\\\"" + szNamespace + "\\\"\";" CRLF;
	}
	szDefinition += "\t}" CRLF;

	//Not found other cases so we supposed attribute is only a simpleType
	for(attr = m_pListAttribute->constBegin(); attr != m_pListAttribute->constEnd(); ++attr) {
		if((*attr)->hasRef()){
			pAttribute = (*attr)->getRef();
		}else{
			pAttribute = *attr;
		}

		if(pAttribute->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
			if(pAttribute->isList()){
				szIterator = "iter_" + QString::number(iIteratorCount++);
				szDefinition += "\tif(_" + pAttribute->getName() + "List" + ".size() > 0) {" CRLF;
				szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\";" CRLF;
				szDefinition += QString("\t\tQList<%0>::const_iterator %1;" CRLF).arg( pSimpleType->getVariableTypeString()).arg(szIterator);
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
		}else if(pAttribute->getType()->getClassType() == Type::TypeComplex){
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
				QString szVariableName = "_" + pAttribute->getName().left(1).toLower() + pAttribute->getName().mid(1);
				szDefinition += "\tif(!" + szVariableName + ".isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \" " + pAttribute->getName() + "=\\\"\" + " + szVariableName + ".serialize() + \"\\\"\";" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}

	if(!getExtensionType().isNull()) {
		QString szExtensionName;
		if(getExtensionType()->getClassType() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getVariableTypeString();
		}else{
			szExtensionName = getExtensionType()->getNameWithNamespace();
		}

		if(isExtensionTypeList()){
			szIterator = "iter_" + QString::number(iIteratorCount++);
			szDefinition += "\tQList<" + szExtensionName + ">::const_iterator " + szIterator + " = constBegin();" CRLF;
			szDefinition += "\twhile(" + szIterator + " != constEnd()){" CRLF;
			szDefinition += "\t\tszValue += (*" + szIterator + ").serialize(" +
					QString(((getExtensionType()->getClassType() == Type::TypeSimple) ? "" : "true")) +
					") + \" \";" CRLF;
			szDefinition += "\t\t++" + szIterator + ";" CRLF;
			szDefinition += "\t}" CRLF;
		}else{
			szDefinition += "\tszValue += " +
					szExtensionName +
					"::serialize(" +
					((getExtensionType()->getClassType() == Type::TypeSimple) ? "" : "true") +
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

		if(pElement->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szIterator = "iter_" + QString::number(iIteratorCount++);
				if(pSimpleType->isEnumeration()){
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getNameWithNamespace()).arg(szIterator);
				}else{
					szDefinition += QString("\tQList<%0>::const_iterator %1;" CRLF).arg(pSimpleType->getVariableTypeString()).arg(szIterator);
				}
				szDefinition += QString("\tfor(%0 = %1List.constBegin(); %0 != %1List.constEnd(); ++%0) {" CRLF).arg(szIterator).arg(pElement->getVariableName());
				szDefinition += QString("\t\tif(!%0->isNull()) {" CRLF).arg(szIterator);
				szDefinition += QString("\t\t\tszValue += %0->serialize();" CRLF).arg(szIterator);
				szDefinition += QString("\t\t}" CRLF);
				szDefinition += "\t}" CRLF;
			} else{

				szDefinition += "\tif(!" + pElement->getVariableName() + ".isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + pElement->getName() + ">\" + " + pElement->getVariableName() + ".serialize() + \"</" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}

		}else if(pElement->getType()->getClassType() == Type::TypeComplex) {
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
					szDefinition += QString("\t\t\tszValue += \"<%0\";" CRLF).arg(pElement->getName());
					szDefinition += QString("\t\t\tszValue += %0->serialize(true);" CRLF).arg(szIterator);
					szDefinition += QString("\t\t\tszValue += \"</%0>\";" CRLF).arg(pElement->getName());
					szDefinition += QString("\t\t}" CRLF);
					szDefinition += "\t}" CRLF;
				}
			} else{
				szDefinition += "\tif(" + ((pElement->isNested() || pElement->isPointer()) ? pElement->getVariableName() + " && " : "") +
						"!" + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "isNull()) {" CRLF;
				szDefinition += "\t\tszValue += \"<" + pElement->getName() + "\" + " + pElement->getVariableName() + ((pElement->isNested() || pElement->isPointer()) ? "->" : ".") + "serialize(true) + \"</" + pElement->getName() + ">\";" CRLF;
				szDefinition += "\t}" CRLF;
			}
		}
	}
	szDefinition += "\tif(!bOnlyContent) {" CRLF;
	szDefinition += "\t\tszValue += \"</%1>\";" CRLF;
	szDefinition += "\t}" CRLF;
	szDefinition += "\treturn szValue;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(getLocalName());
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
		if(getExtensionType()->getClassType() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getVariableTypeString();
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

		if( pAttribute->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
			if(pAttribute->isList()){
				szDefinition += "\tif(!element.attributeNode(\"" + pAttribute->getName() + "\").isNull()) {" CRLF;
				szDefinition += "\t\tQDomAttr attribute = element.attributeNode(\"" + pAttribute->getName() + "\");" CRLF;
				szDefinition += "\t\tQString szAttrValue = attribute.value();" CRLF;
				szDefinition += "\t\tif(!szAttrValue.isEmpty()){" CRLF;
				szDefinition += "\t\t\tQStringList szValues = szAttrValue.split(\" \");" CRLF;
				szDefinition += "\t\t\tfor(int i = 0; i < szValues.size(); ++i){" CRLF;
				szDefinition += "\t\t\t\tif(!szValues[i].isEmpty()){" CRLF;
				szDefinition += "\t\t\t\t\t" + pSimpleType->getVariableTypeString() + " tmp;" CRLF;
				szDefinition += "\t\t\t\t\ttmp.setValue(szValues[i]);" CRLF;
				szDefinition += "\t\t\t\t\tadd" + pAttribute->getName() + "(tmp);" CRLF;
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

		if(pElement->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());

			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szDefinition += "\t\tif(child.tagName() == \"" + getNamespace() + ":" + pElement->getName() + "\") {" CRLF;
				if(pSimpleType->isEnumeration()){
					szDefinition += "\t\t\t" + pSimpleType->getNameWithNamespace() + " item;" CRLF;
				}else{
					szDefinition += "\t\t\t" + pSimpleType->getVariableTypeString() + " item;" CRLF;
				}
				szDefinition += "\t\t\titem.deserialize(child);" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + "List.append(item);" CRLF;
				szDefinition += "\t\t}" CRLF;

			} else{
				szDefinition += "\t\tif((child.tagName() == \"" + getNamespace() + ":" + pElement->getName() + "\")" +
						" || child.tagName().endsWith(\":" + pElement->getName() + "\"" + ")){" CRLF;
				szDefinition += "\t\t\t" + pElement->getVariableName() + ".deserialize(child);" CRLF;
				szDefinition += "\t\t}" CRLF;
			}

		}else if(pElement->getType()->getClassType() == Type::TypeComplex) {
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
		if(getExtensionType()->getClassType() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(getExtensionType());
			szExtensionName = pSimpleType->getVariableTypeString();
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

		if( pAttribute->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());
			if(pAttribute->isList()){
				szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
			}else{
				szCond += pSimpleType->getVariableName() + ".isNull() && ";
			}

		}else if( pAttribute->getType()->getClassType() == Type::TypeComplex) {
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());
			if(pAttribute->isList()){
				szCond += "(_" + pAttribute->getName() + "List.size() > 0) && ";
			}else{
				QString szVariableName = "_" + pAttribute->getName().left(1).toLower() + pAttribute->getName().mid(1);
				szCond += szVariableName + ".isNull() && ";
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
		if(pElement->getType()->getClassType() == Type::TypeSimple) {
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());
			if(pElement->getMaxOccurs() > 1 || pElement->getMaxOccurs() == -1) {
				szCond += pElement->getVariableName() + "List.isEmpty() && ";
			}else{
				szCond += pElement->getVariableName() + ".isNull() && ";
			}

		}else if(pElement->getType()->getClassType() == Type::TypeComplex) {
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

QString ComplexType::getVariableDeclaration(const QString& szName) const
{
	QString szVarName;
	QString szDeclaration;
	if(!szName.isEmpty()){
		szVarName = "_" + szName.left(1).toLower() + szName.mid(1);
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

