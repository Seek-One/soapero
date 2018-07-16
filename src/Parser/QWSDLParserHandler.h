/*
 * QWSDLParserHandler.h
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#ifndef QWSDLPARSERHANDLER_H_
#define QWSDLPARSERHANDLER_H_

#include <QSharedPointer>
#include <QStack>
#include <QXmlDefaultHandler>
#include <QXmlAttributes>

#include "Model/RequestResponseElement.h"
#include "Model/Message.h"
#include "Model/Operation.h"
#include "Model/Service.h"
#include "Model/Type.h"
#include "WSDLSections.h"

class QWSDLParserHandler : public QXmlDefaultHandler
{
public:
	QWSDLParserHandler(QMap<QString, QStringList>* pNamespaceRoutingMap = NULL, const QString& szOutNamespace = QString());
	virtual ~QWSDLParserHandler();

	void initXMLAttributes();		// https://www.w3.org/2001/xml.xsd

    virtual bool endDocument();
	virtual bool startElement(const QString &namespaceURI,
			const QString &localName,
			const QString &qName,
			const QXmlAttributes &attributes);
	virtual bool endElement(const QString &namespaceURI,
			const QString &localName,
			const QString &qName);
	virtual bool characters(const QString &str);
	virtual bool fatalError(const QXmlParseException &exception);

	TypeListSharedPtr getTypeList() const;
	RequestResponseElementListSharedPtr getRequestResponseElementList() const;
	ServiceSharedPtr getService() const;
	AttributeListSharedPtr getAttributeList() const;
	ElementListSharedPtr getElementList() const;

private:
	ElementSharedPtr getElementByRef(const QString& szRef);
	TypeSharedPtr getTypeByName(const QString& szLocalName, const QString& szNamespace = QString(), const TypeListSharedPtr& pListIgnoredTypes = TypeList::create());

	void loadFromHttp(const QString& szURL, const QString& szNamespace = QString());
	void loadFromFile(const QString& szFileName, const QString& szNamespace = QString());

	void addCurrentType(const TypeSharedPtr& pCurrentType);
	void leaveCurrentType();
	const TypeSharedPtr currentType() const;

	void enterSection(QString szSection);
	void enterSection(Section::Name section = Section::Other);
	void leaveSection();
	bool inSection(Section::Name section) const;
	Section::Name parentSection() const;
	Section::Name currentSection() const;

private:
    QString m_szCurrentType;
    QString m_szCurrentText;
    QString m_szCurrentOperationName;

    QString m_szCurrentNamespacePrefix;
    QString m_szTargetNamespacePrefix;

    QMap<QString, QStringList>* m_pNamespaceRoutingMap;
    QMap<QString, QString> m_pNamespaceDeclarationMap;
    QString m_szOutNamespace;
    bool m_bOwnNamespaceRoutingMap;

    TypeListSharedPtr m_pListTypes;
    RequestResponseElementListSharedPtr m_pListRequestResponseElements;
    MessageListSharedPtr m_pListMessages;
    OperationListSharedPtr m_pListOperations;
    AttributeListSharedPtr m_pListAttributes;
    ElementListSharedPtr m_pListElements;

    ServiceSharedPtr m_pService;

    QStack<TypeSharedPtr> m_currentTypeStack;
    RequestResponseElementSharedPtr m_pCurrentRequestResponseElement;
    MessageSharedPtr m_pCurrentMessage;
    OperationSharedPtr m_pCurrentOperation;
    AttributeSharedPtr m_pCurrentAttribute;
    ElementSharedPtr m_pCurrentElement;

    QStack<Section::Name> m_sectionStack;
};

#endif /* QWSDLPARSERHANDLER_H_ */
