/*
 * SimpleType.cpp
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#include "../Utils/StringUtils.h"
#include "../Utils/ModelUtils.h"

#include "SimpleType.h"

#include "../common.h"

SimpleType::SimpleType() : Type(CategoryType, Type::TypeSimple)
{
	m_iVariableType = Unknown;
	m_iMaxLength = -1;
	m_iMinLength = -1;
	m_iMinInclusive = -1;
	m_bRestricted = false;
	m_bIsUnion = false;
}

SimpleType::~SimpleType() {}

SimpleTypeSharedPtr SimpleType::create()
{
	return SimpleTypeSharedPtr(new SimpleType());
}

bool SimpleType::hasVariableType() const
{
	return (m_iVariableType != Unknown);
}

void SimpleType::setVariableType(VariableType type)
{
	m_iVariableType = type;
}

void SimpleType::setVariableTypeFromString(const QString& szNamespacePrefix, const QString& szType)
{
	if(szType == (szNamespacePrefix + ":string")) {
		m_iVariableType = String;
	} else if(szType == (szNamespacePrefix + ":anyURI")) {
		m_iVariableType = AnyURI;
	} else if(szType == (szNamespacePrefix + ":int")) {
		m_iVariableType = Int;
	} else if(szType == (szNamespacePrefix + ":integer")) {
		m_iVariableType = Int;
	} else if(szType == (szNamespacePrefix + ":unsignedInt")) {
		m_iVariableType = UnsignedInt;
	} else if(szType == (szNamespacePrefix + ":boolean")) {
		m_iVariableType = Boolean;
	}else if(szType == (szNamespacePrefix + ":duration")) {
		m_iVariableType = Duration;
	}else if(szType == (szNamespacePrefix + ":base64Binary")) {
		m_iVariableType = Base64Binary;
	}else if(szType == (szNamespacePrefix + ":float")){
		m_iVariableType = Float;
	}else if(szType == (szNamespacePrefix + ":dateTime")){
		m_iVariableType = DateTime;
	}else if(szType == (szNamespacePrefix + ":QName")){
		m_iVariableType = QName;
	}else if(szType == (szNamespacePrefix + ":NCName")){
		m_iVariableType = NCName;
	}else if(szType == (szNamespacePrefix + ":token")){
		m_iVariableType = Token;
	}else if(szType == (szNamespacePrefix + ":unsignedLong")){
		m_iVariableType = UnsignedLong;
	}else if(szType == (szNamespacePrefix + ":anyType")){
		m_iVariableType = AnyType;
	}else if(szType == (szNamespacePrefix + ":nonNegativeInteger")){
		m_iVariableType = NonNegativeInteger;
	}else if(szType == (szNamespacePrefix + ":hexBinary")){
		m_iVariableType = HexBinary;
	}else if(szType == (szNamespacePrefix + ":double")){
		m_iVariableType = Double;
	}else if(szType == (szNamespacePrefix + ":anySimpleType")){
		m_iVariableType = AnySimpleType;
	}else{
		m_iVariableType = Unknown;
	}
	m_szVariableTypeNamespace = szNamespacePrefix;
}

SimpleType::VariableType SimpleType::getVariableType()const
{
	return m_iVariableType;
}

QString SimpleType::getVariableTypeNameString(SimpleType::VariableType iVariableType)
{
	switch(iVariableType) {
	case String:
		return "String";
	case Int:
		return "Integer";
	case UnsignedInt:
		return "UnsignedInteger";
	case Boolean:
		return "Boolean";
	case Duration:
		return "Duration";
	case Base64Binary:
		return "Base64Binary";
	case AnyURI:
		return "AnyURI";
	case Float:
		return "Float";
	case DateTime:
		return "DateTime";
	case QName:
		return "QName";
	case NCName:
		return "NCName";
	case Token:
		return "Token";
	case UnsignedLong:
		return "UnsignedLong";
	case AnyType:
		return "AnyType";
	case NonNegativeInteger:
		return "NonNegativeInteger";
	case HexBinary:
		return "HexBinary";
	case Double:
		return "Double";
	case AnySimpleType:
		return "AnySimpleType";
	default:
		break;
	}
	return QString();
}

QString SimpleType::getCPPTypeNameString() const
{
	if(isEnumeration()) {
		return getNameWithNamespace();
	}else{
		if(m_iVariableType == Custom){
			return (m_szCustomNamespace + getCategoryNamespace() + "::" + m_szCustomName);
		}else{
			return "XS::" + getVariableTypeNameString(m_iVariableType);
		}
	}
}

QString SimpleType::getCPPTypeNameValuesString() const
{
	if(isEnumeration()) {
		return getNameWithNamespace() + "::Values";
	}else{
		return getCPPTypeNameString();
	}
}

QString SimpleType::getVariableTypeString() const
{
	if(isEnumeration()) {
		return getLocalName() + "::Values";
	}else{
		if(m_iVariableType == Custom){
			return (m_szCustomNamespace + "::" + m_szCustomName);
		}else{
			return getVariableTypeNameString(m_iVariableType);
		}
	}
}

QString SimpleType::getVariableTypeFilenameString() const
{
	if(isEnumeration()) {
		return getQualifiedName();

	}else{
		switch(m_iVariableType) {
		case String:
			return "XSString";
		case Int:
			return "XSInteger";
		case UnsignedInt:
			return "XSUnsignedInteger";
		case Boolean:
			return "XSBoolean";
		case Duration:
			return "XSDuration";
		case Base64Binary:
			return "XSBase64Binary";
		case AnyURI:
			return "XSAnyURI";
		case Float:
			return "XSFloat";
		case DateTime:
			return "XSDateTime";
		case QName:
			return "XSQName";
		case NCName:
			return "XSNCName";
		case Token:
			return "XSToken";
		case UnsignedLong:
			return "XSUnsignedLong";
		case AnyType:
			return "XSAnyType";
		case NonNegativeInteger:
			return "XSNonNegativeInteger";
		case HexBinary:
			return "XSHexBinary";
		case Double:
			return "XSDouble";
		case AnySimpleType:
			return "XSAnySimpleType";

		case Custom:
			return (m_szCustomNamespace + m_szCustomName);
		default:
			return QString();
		}
	}
}

const QString& SimpleType::getVariableTypeNamepace() const
{
	return m_szVariableTypeNamespace;
}

void SimpleType::setCustomNamespace(const QString& szCustomNamespace)
{
	m_szCustomNamespace = szCustomNamespace;
}

const QString& SimpleType::getCustomNamespace() const
{
	return m_szCustomNamespace;
}

void SimpleType::setCustomName(const QString& szCustomName)
{
	m_szCustomName = szCustomName;
}

const QString& SimpleType::getCustomName() const
{
	return m_szCustomName;
}

bool SimpleType::isEnumeration() const
{
	return m_bRestricted &&
			((m_iVariableType == String) ||
					(m_iVariableType == Base64Binary) ||
					(m_iVariableType == AnyURI) ||
					(m_iVariableType == QName) ||
					(m_iVariableType == NCName) ||
					(m_iVariableType == Token)) &&
			(m_listEnumerationValues.count() > 0);
}


void SimpleType::setIsUnion(bool bIsUnion)
{
	m_bIsUnion = bIsUnion;
}

bool SimpleType::isUnion() const
{
	return (m_bIsUnion);
}

QString SimpleType::getExportedNamespace() const
{
	if(hasVariableType()){
		if(isEnumeration()){
			return getNamespace();
		}else if(m_iVariableType == Custom){
			return m_szCustomNamespace;
		}else{
			return getVariableTypeNamepace();
		}
	}
	return getNamespace();
}

QString SimpleType::getExportedTypename() const
{
	if(hasVariableType()){
		if(isEnumeration()){
			return getLocalName();
		}else if(m_iVariableType == Custom){
			return m_szCustomName;
		}else{
			return getVariableTypeString();
		}
	}
	return getLocalName();
}

QString SimpleType::getEnumConvertDeclaration() const
{
	QString szFuncName = ModelUtils::getCapitalizedName(getLocalName());

	QString szDeclaration;
	if(isEnumeration()) {
		szDeclaration += "void set%0FromString(const QString& szValue);" CRLF;
		szDeclaration += "\tQString get%0ToString() const;";
		szDeclaration = szDeclaration.arg(szFuncName);
	}
	return szDeclaration;
}

QString SimpleType::getVariableDeclaration() const
{
	QString szDeclaration;
	szDeclaration += getCPPTypeNameValuesString();
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
				szDeclaration += StringUtils::removeNonAlphaNum(m_listEnumerationValues[i]);
			} else {
				szDeclaration += ", " + StringUtils::removeNonAlphaNum(m_listEnumerationValues[i]);
			}
		}
		if(!bHasUnknownValue) {
			szDeclaration += ", Unknown";
		}
		szDeclaration += "};";
	}
	return szDeclaration;
}

QString SimpleType::getEnumConvertDefinition(const QString& szClassname) const
{
	QString szDefinition;

	QString szFuncName = ModelUtils::getCapitalizedName(getLocalName());

	if(isEnumeration()) {

		szDefinition += ""
		"void %0::set%1FromString(const QString& szValue)" CRLF
		"{" CRLF
		"\tif((szValue == \"" + getEnumerationValues()[0] + "\") || szValue.endsWith(\":" + StringUtils::getLocalNameIfPossible(getEnumerationValues()[0]) + "\")) {" CRLF
		"\t\t %2 = %0::" + StringUtils::removeNonAlphaNum(getEnumerationValues()[0]) + ";" CRLF
		"\t}";

		for(int i=1; i < getEnumerationValues().count(); ++i) {
			szDefinition += ""
			"else if((szValue == \"" + getEnumerationValues()[i] + "\") || szValue.endsWith(\":" + StringUtils::getLocalNameIfPossible(getEnumerationValues()[i]) + "\")) {" CRLF
			"\t\t%2 = %0::" + StringUtils::removeNonAlphaNum(getEnumerationValues()[i]) + ";" CRLF
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
			"\tcase " + StringUtils::removeNonAlphaNum(getEnumerationValues()[i]) + ":" CRLF
			"\t\treturn \"" + getEnumerationValues()[i] + "\";" CRLF;
		}
		szDefinition += "\tdefault:" CRLF;
		szDefinition += "\t\treturn \"Unknown\";" CRLF;
		szDefinition += "\t}" CRLF;
		szDefinition += "}" CRLF;

		szDefinition = szDefinition.arg(szClassname).arg(szFuncName).arg(getVariableName());
	}
	return szDefinition;
}


QString SimpleType::getVariableName() const
{
	return StringUtils::secureString(QString("_" + getLocalName().left(1).toLower() + getLocalName().mid(1)));
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

void SimpleType::addUnionType(const TypeRefSharedPtr& pTypeRef)
{
	m_listUnionTypeRef.append(pTypeRef);
}

const TypeRefList& SimpleType::getUnionTypes() const
{
	return m_listUnionTypeRef;
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


