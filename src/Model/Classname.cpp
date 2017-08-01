/*
 * Classname.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include <QStringList>

#include "Model/Classname.h"

Classname::Classname()
{

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
	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper() + "::" + m_szLocalName;
	}
}

QString Classname::getQualifiedName() const
{
	if(m_szNamespace.isEmpty()) {
		return m_szLocalName;
	}else{
		return m_szNamespace.toUpper() + m_szLocalName;
	}
}

QString Classname::getNamespace() const
{
	return m_szNamespace;
}

QString Classname::getLocalName() const
{
	return m_szLocalName;
}

