//
// Created by ebeuque on 15/09/2021.
//

#include "QWSDLData.h"

QWSDLData::QWSDLData()
{

}

QWSDLData::~QWSDLData()
{

}

void QWSDLData::addLoadedURI(const QString& szURI)
{
	m_listLoadedURI.append(szURI);
}

bool QWSDLData::hasLoadedURI(const QString& szURI) const
{
	return m_listLoadedURI.contains(szURI);
}

void QWSDLData::addNamespaceDeclaration(const QString& szNamespace, const QString& szNamespaceURI)
{
	m_listNamespaceDeclarations.insert(szNamespace, szNamespaceURI);
}

bool QWSDLData::hasNamespaceDeclaration(const QString& szNamespace) const
{
	return m_listNamespaceDeclarations.contains(szNamespace);
}

QString QWSDLData::getNamespaceDeclaration(const QString& szNamespace) const
{
	return m_listNamespaceDeclarations.value(szNamespace);
}

const QWSDLNamespaceDeclarations& QWSDLData::getNamespaceDeclarations() const
{
	return m_listNamespaceDeclarations;
}