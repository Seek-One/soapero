/*
 * SimpleType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "SimpleType.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

SimpleType::SimpleType()
	:Type(Type::TypeSimple)
{
	m_variableType = Unknown;
	m_iMaxLength = -1;
	m_iMinLength = -1;
	m_iMinInclusive = -1;
	m_bRestricted = false;
}

SimpleType::~SimpleType() {}

SimpleTypeSharedPtr SimpleType::create()
{
	return SimpleTypeSharedPtr(new SimpleType());
}

void SimpleType::setVariableType(VariableType type)
{
	m_variableType = type;
}

void SimpleType::setVariableTypeFromString(const QString& szType)
{
	if(szType == "xs:string") {
		m_variableType = String;
	} else if(szType == "xs:int") {
		m_variableType = Int;
	} else if(szType == "xs:integer") {
		m_variableType = Int;
	} else if(szType == "xs:unsignedInt") {
		m_variableType = UnsignedInt;
	} else if(szType == "xs:boolean") {
		m_variableType = Boolean;
	}else if(szType == "xs:duration") {
		m_variableType = Duration;
	}
}

SimpleType::VariableType SimpleType::getVariableType()const
{
	return m_variableType;
}

QString SimpleType::getVariableTypeString()const
{
	if(isEnumeration()) {
		return getLocalName() + "::Values";

	}else{
		switch(m_variableType) {
		case String:
			return "XS::String";
			break;
		case Int:
			return "XS::Integer";
			break;
		case UnsignedInt:
			return "XS::UnsignedInteger";
			break;
		case Boolean:
			return "XS::Boolean";
			break;
		case Duration:
			return "XS::Duration";
			break;
		default:
			return QString();
		}
	}
}

QString SimpleType::getVariableTypeFilenameString() const
{
	if(isEnumeration()) {
		return getQualifiedName();

	}else{
		switch(m_variableType) {
		case String:
			return "XSString";
			break;
		case Int:
			return "XSInteger";
			break;
		case UnsignedInt:
			return "XSUnsignedInteger";
			break;
		case Boolean:
			return "XSBoolean";
			break;
		case Duration:
			return "XSDuration";
			break;
		default:
			return QString();
		}
	}
}

bool SimpleType::isEnumeration() const
{
	return m_bRestricted && m_variableType == String && m_listEnumerationValues.count() > 0;
}

QString SimpleType::getSetterDeclaration() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration;
	if(isEnumeration()) {
		szDeclaration = "void set%0(%1 %2);";
	}else{
		szDeclaration = "void set%0(const %1& %2);";
	}
	return szDeclaration.arg(getLocalName()).arg(getVariableTypeString()).arg(szVarName);
}

QString SimpleType::getSetterDeclarationForComplexType() const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDeclaration = "void set%0(const %1& %2);";

	return szDeclaration.arg(getLocalName()).arg(isEnumeration() ? getNameWithNamespace() : getVariableTypeString()).arg(szVarName);
}

QString SimpleType::getGetterDeclaration() const
{
	QString szDeclaration;
	if(isEnumeration()) {
		szDeclaration = "%0 get%1() const;";
	}else{
		szDeclaration = "const %0& get%1() const;";
	}
	return szDeclaration.arg(getVariableTypeString()).arg(getLocalName());
}

QString SimpleType::getGetterDeclarationForComplexType() const
{
	QString szDeclaration;
	if(isEnumeration()) {
		szDeclaration = "%0 get%1() const;";
	}else{
		szDeclaration = "const %0& get%1() const;";
	}
	return szDeclaration.arg(isEnumeration() ? getNameWithNamespace() : getVariableTypeString()).arg(getLocalName());
}

QString SimpleType::getSerializerDeclaration() const
{
	return "QString serialize() const;";
}

QString SimpleType::getDeserializerDeclaration() const
{
	return "void deserialize(const QDomElement& element);";
}

QString SimpleType::getEnumConvertDeclaration() const
{
	QString szDeclaration;
	if(isEnumeration()) {

		szDeclaration += "void set%0FromString(const QString& szValue);" CRLF;
		szDeclaration += "\tQString get%0ToString() const;";
		szDeclaration = szDeclaration.arg(getLocalName());
	}
	return szDeclaration;
}

QString SimpleType::getIsNullDeclaration() const
{
	return "bool isNull() const;";
}

QString SimpleType::getVariableDeclaration() const
{
	QString szDeclaration;
	szDeclaration += getVariableTypeString();
	szDeclaration += " ";
	szDeclaration += getVariableName();
	szDeclaration += ";";

	return szDeclaration;
}

QString SimpleType::getVariableDeclarationForComplexType() const
{
	QString szDeclaration;
	if(!isEnumeration()) {
		szDeclaration += getVariableTypeString();
	} else {
		szDeclaration += getNameWithNamespace();
	}
	szDeclaration += " ";
	szDeclaration += getVariableName();
	szDeclaration += ";";

	return szDeclaration;
}

QString SimpleType::getEnumerationDeclaration() const
{
	bool bHasUnknownValue = false;
	QString szDeclaration;
	if(m_bRestricted) {
		szDeclaration += "enum Values {";
		for(int i=0; i < m_listEnumerationValues.count(); ++i) {
			if(m_listEnumerationValues[i] == "Unknown") {
				bHasUnknownValue = true;
			}
			if(i==0) {
				szDeclaration += m_listEnumerationValues[i];
			} else {
				szDeclaration += ", " + m_listEnumerationValues[i];
			}
		}
		if(!bHasUnknownValue) {
			szDeclaration += ", Unknown";
		}
		szDeclaration += "};";
	}
	return szDeclaration;
}

QString SimpleType::getSetterDefinition(const QString& szClassname) const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDefinition = "";
	if(isEnumeration()) {
		szDefinition = "void %0::set%1(%2 %3)" CRLF;
	}else{
		szDefinition = "void %0::set%1(const %2& %3)" CRLF;
	}
	szDefinition += "{" CRLF;
	szDefinition += "\t%4 = %5;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(getLocalName()).arg(getVariableTypeString())
						.arg(szVarName).arg(getVariableName()).arg(szVarName);
}

QString SimpleType::getSetterDefinitionForComplexType(const QString& szClassname) const
{
	QString szVarName = getLocalName().left(1).toLower() + getLocalName().mid(1);

	QString szDefinition = "void %0::set%1(const %2& %3)" CRLF;

	szDefinition += "{" CRLF;
	szDefinition += "\t%4 = %5;" CRLF;
	szDefinition += "}" CRLF;

	return szDefinition.arg(szClassname).arg(getLocalName()).arg(getNameWithNamespace())
						.arg(szVarName).arg(getVariableName()).arg(szVarName);
}

QString SimpleType::getGetterDefinition(const QString& szClassname) const
{
	QString szDefinition;
	if(isEnumeration()) {
		szDefinition = ""
		"%0 %1::get%2() const" CRLF
		"{" CRLF
		"\treturn %3;" CRLF
		"}" CRLF;
	} else {
		szDefinition = ""
		"const %0& %1::get%2() const" CRLF
		"{" CRLF
		"\treturn %3;" CRLF
		"}" CRLF;
	}

	return szDefinition.arg(getVariableTypeString()).arg(szClassname).arg(getLocalName()).arg(getVariableName());
}

QString SimpleType::getGetterDefinitionForComplexType(const QString& szClassname) const
{
	QString szDefinition = ""
	"const %0& %1::get%2() const" CRLF
	"{" CRLF
	"\treturn %3;" CRLF
	"}" CRLF;

	return szDefinition.arg(getNameWithNamespace()).arg(szClassname).arg(getLocalName()).arg(getVariableName());
}

QString SimpleType::getSerializerDefinition(const QString& szClassname) const
{
	if(isEnumeration()) {

		QString szDefinition = ""
		"QString %0::serialize() const" CRLF
		"{" CRLF
		"\tif(!isNull()) {" CRLF
		"\t\treturn \"<%1>\" + get%2ToString() + \"</%1>\";" CRLF
		"\t}" CRLF
		"\t return \"\";" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getLocalName()).arg(getLocalName());

	} else {
		QString szDefinition = ""
		"QString %0::serialize() const" CRLF
		"{" CRLF
		"\tif(!isNull()) {" CRLF
		"\t\treturn \"<%1>\" + %2.serialize() + \"</%1>\";" CRLF
		"\t}" CRLF
		"\t return \"\";" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getLocalName()).arg(getVariableName());
	}
}

QString SimpleType::getDeserializerDefinition(const QString& szClassname) const
{
	if(isEnumeration()) {

		QString szDefinition = ""
		"void %0::deserialize(const QDomElement& element)" CRLF
		"{" CRLF
		"\tset%1FromString(element.text().trimmed());" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getLocalName());

	} else {
		QString szDefinition = ""
		"void %0::deserialize(const QDomElement& element)" CRLF
		"{" CRLF
		"\tQDomElement child = element.firstChild().toElement();" CRLF
		"\twhile(!child.isNull()) {" CRLF
		"\t\tif(child.tagName() == \"%1\") {" CRLF
		"\t\t%2.deserialize(child);" CRLF
		"\t\t}" CRLF
		"\tchild = child.nextSibling().toElement();" CRLF
		"\t}" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getTagQualifiedName()).arg(getVariableName());
	}
}

QString SimpleType::getEnumConvertDefinition(const QString& szClassname) const
{
	QString szDefinition;
	if(isEnumeration()) {

		szDefinition += ""
		"void %0::set%1FromString(const QString& szValue)" CRLF
		"{" CRLF
		"\tif(szValue == \"" + getEnumerationValues()[0] + "\") {" CRLF
		"\t\t %2 = %0::" + getEnumerationValues()[0] + ";" CRLF
		"\t}";

		for(int i=1; i < getEnumerationValues().count(); ++i) {
			szDefinition += ""
			"else if(\"" + getEnumerationValues()[i] + "\" == szValue) {" CRLF
			"\t\t%2 = %0::" + getEnumerationValues()[i] + ";" CRLF
			"\t}";
		}
		szDefinition += " else {" CRLF;
		szDefinition += "\t\t%2 = %0::Unknown;" CRLF;
		szDefinition += "\t}" CRLF;
		szDefinition += CRLF;
		szDefinition += "}" CRLF CRLF;

		szDefinition += ""
		"QString %0::get%1ToString() const" CRLF
		"{" CRLF
		"\tswitch(%2) {" CRLF;

		for(int i=0; i < getEnumerationValues().count(); ++i) {
			szDefinition += ""
			"\tcase " + getEnumerationValues()[i] + ":" CRLF
			"\t\treturn \"" + getEnumerationValues()[i] + "\";" CRLF;
		}
		szDefinition += "\tdefault:" CRLF;
		szDefinition += "\t\treturn \"Unknown\";" CRLF;
		szDefinition += "\t}" CRLF;
		szDefinition += "}" CRLF;

		szDefinition = szDefinition.arg(szClassname).arg(getLocalName()).arg(getVariableName());
	}
	return szDefinition;
}

QString SimpleType::getIsNullDefinition(const QString& szClassname) const
{
	if(isEnumeration()) {

		QString szDefinition = ""
		"bool %0::isNull() const" CRLF
		"{" CRLF
		"\t return %1 == Unknown;" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getVariableName());

	} else {
		QString szDefinition = ""
		"bool %0::isNull() const" CRLF
		"{" CRLF
		"\t return %1.isNull();" CRLF
		"}" CRLF;

		return szDefinition.arg(szClassname).arg(getVariableName());
	}
}

QString SimpleType::getVariableName() const
{
	return "_" + getLocalName().left(1).toLower() + getLocalName().mid(1);
}

QString SimpleType::getVariableIsNullName() const
{
	return "_bIsNull";
}

void SimpleType::setRestricted(bool bRestricted)
{
	m_bRestricted = bRestricted;
}

bool SimpleType::isRestricted() const
{
	return m_bRestricted;
}

void SimpleType::addEnumerationValue(const QString& szValue)
{
	m_listEnumerationValues.append(szValue);
}

QStringList SimpleType::getEnumerationValues() const
{
	return m_listEnumerationValues;
}

void SimpleType::setMaxLength(int iMaxLength)
{
	m_iMaxLength = iMaxLength;
}

int SimpleType::getMaxLength() const
{
	return m_iMaxLength;
}

void SimpleType::setMinLength(int iMinLength)
{
	m_iMinLength = iMinLength;
}

int SimpleType::getMinLength() const
{
	return m_iMinLength;
}

void SimpleType::setMinInclusive(int iMinInclusive)
{
	m_iMinInclusive = iMinInclusive;
}

int SimpleType::getMinInclusive() const
{
	return m_iMinInclusive;
}

SimpleTypeList::SimpleTypeList()
	:QList<SimpleTypeSharedPtr>()
{

}

SimpleTypeList::~SimpleTypeList()
{

}

SimpleTypeListSharedPtr SimpleTypeList::create()
{
	return SimpleTypeListSharedPtr(new SimpleTypeList());
}


