/*
 * Classname.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "../Utils/StringUtils.h"
#include "Model/Classname.h"

Classname::Classname(ClassCategory iCategory)
{
	m_iCategory = iCategory;
}

Classname::Classname(const Classname& other)
{
	m_iCategory = other.m_iCategory;
	m_szLocalName = other.m_szLocalName;
	m_szNamespace = other.m_szNamespace;
	m_szNamespaceUri = other.m_szNamespaceUri;
}

Classname::~Classname()
{

}

Classname::ClassCategory Classname::getClassCategory() const
{
	return m_iCategory;
}

QString Classname::getLocalName(bool bSafe) const
{
	if(bSafe){
		QString szSafeLocalName = StringUtils::secureString(m_szLocalName);
		return szSafeLocalName;
	}
	return m_szLocalName;
}

void Classname::setLocalName(const QString& szLocalName)
{
	m_szLocalName = szLocalName;
}

const QString& Classname::getNamespace() const
{
	return m_szNamespace;
}

void Classname::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

const QString& Classname::getNamespaceUri() const
{
	return m_szNamespaceUri;
}

void Classname::setNamespaceUri(const QString& szNamespaceUri)
{
	m_szNamespaceUri = szNamespaceUri;
}

const QString& Classname::getSchemaUri() const
{
	return m_szSchemaUri;
}

void Classname::setSchemaUri(const QString& szSchemaUri)
{
	m_szSchemaUri = szSchemaUri;
}

void Classname::setName(const QString& szName)
{
	if(szName.contains(":")) {
		const QStringList listParts = szName.split(":");
		m_szNamespace = listParts[0];
		m_szLocalName = listParts[1];
	}else{
		m_szLocalName = szName;
	}
}

QString Classname::getQualifiedName() const
{
	QString szSafeLocalName = StringUtils::secureString(m_szLocalName);

	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper().replace("-", "_") + getCategoryNamespace() + "::" + szSafeLocalName;
	}
}

void Classname::setQualifedName(const QString& szNamespace, const QString& szLocalName)
{
	m_szNamespace = szNamespace;
	m_szLocalName = szLocalName;
}

QString Classname::getTagQualifiedName() const
{
	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace + ":" + m_szLocalName;
	}
}

QString Classname::getNameWithNamespace() const
{
	QString szSafeLocalName = StringUtils::secureString(m_szLocalName);

	if(m_szNamespace.isEmpty()) {
		return szSafeLocalName;
	}else{
		return m_szNamespace.toUpper().replace("-", "_") + getCategoryNamespace() + "::" + szSafeLocalName;
	}
}

QString Classname::getCategoryNamespace() const
{
	if(m_iCategory == CategoryType){
		return "::TYPES";
	}
	if(m_iCategory == CategoryMessage){
		return "::MSG";
	}
	return "";
}
