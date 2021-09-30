/*
 * Classname.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "../Utils/StringUtils.h"
#include "Model/Classname.h"

Classname::Classname()
{

}

Classname::Classname(const Classname& other)
{
	m_szLocalName = other.m_szLocalName;
	m_szNamespace = other.m_szNamespace;
	m_szNamespaceUri = other.m_szNamespaceUri;
}

Classname::~Classname()
{

}

void Classname::setQualifedName(const QString& szNamespace, const QString& szLocalName)
{
	m_szNamespace = szNamespace;
	m_szLocalName = szLocalName;
}

void Classname::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

void Classname::setNamespaceUri(const QString& szNamespaceUri)
{
	m_szNamespaceUri = szNamespaceUri;
}

void Classname::setLocalName(const QString& szLocalName)
{
	m_szLocalName = szLocalName;
}

void Classname::setName(const QString& szName)
{
	if(szName.contains(":")) {
		m_szNamespace = szName.split(":")[0];
		m_szLocalName = szName.split(":")[1];
	}else{
		m_szLocalName = szName;
	}
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
		return m_szNamespace.toUpper().replace("-", "_") + "::" + szSafeLocalName;
	}
}

QString Classname::getQualifiedName() const
{
	QString szSafeLocalName = StringUtils::secureString(m_szLocalName);

	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper().replace("-", "_") + szSafeLocalName;
	}
}

const QString& Classname::getNamespace() const
{
	return m_szNamespace;
}

const QString& Classname::getNamespaceUri() const
{
	return m_szNamespaceUri;
}

QString Classname::getLocalName(bool bSafe) const
{
	if(bSafe){
		QString szSafeLocalName = StringUtils::secureString(m_szLocalName);
		return szSafeLocalName;
	}
	return m_szLocalName;
}
