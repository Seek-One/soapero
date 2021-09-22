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

void QWSDLData::setTypeList(const TypeListSharedPtr& pListType)
{
	m_pListType = pListType;
}

void QWSDLData::addType(const TypeSharedPtr& pType)
{
	if(m_pListType){
		m_pListType->add(pType);
	}
}

TypeSharedPtr QWSDLData::getTypeByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes)
{
	if(m_pListType){
		return m_pListType->getByName(szLocalName, szNamespace, pListIgnoredTypes);
	}
	return TypeSharedPtr();
}

TypeRefSharedPtr QWSDLData::getTypeRefByTypeName(const QString& szTypeName, const QString& szNamespace)
{
	return m_listTypeRef.getByTypeName(szTypeName, szNamespace);
}