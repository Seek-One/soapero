//
// Created by ebeuque on 15/09/2021.
//

#ifndef COM_JET1OEIL_SOAPERO_QWSDLDATA_H
#define COM_JET1OEIL_SOAPERO_QWSDLDATA_H

#include <QString>
#include <QList>
#include <QMap>

#include "Model/TypeRef.h"

typedef QMap<QString, QString> QWSDLNamespaceDeclarations;

class QWSDLData
{
public:
	QWSDLData();
	virtual ~QWSDLData();

	// Loaded URI
	void addLoadedURI(const QString& szURI);
	bool hasLoadedURI(const QString& szURI) const;

	// Namespace declarations
	void addNamespaceDeclaration(const QString& szNamespace, const QString& szNamespaceURI);
	bool hasNamespaceDeclaration(const QString& szNamespace) const;
	QString getNamespaceDeclaration(const QString& szNamespace) const;
	const QWSDLNamespaceDeclarations& getNamespaceDeclarations() const;

	// Type list
	TypeRefSharedPtr getTypeRefByTypeName(const QString& szTypeName, const QString& szNamespace = QString());

private:
	QList<QString> m_listLoadedURI;

	QWSDLNamespaceDeclarations m_listNamespaceDeclarations;

	TypeRefList m_listTypeRef;
};


#endif //COM_JET1OEIL_SOAPERO_QWSDLDATA_H
