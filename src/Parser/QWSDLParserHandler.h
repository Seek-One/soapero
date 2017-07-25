/*
 * QWSDLParserHandler.h
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#ifndef QWSDLPARSERHANDLER_H_
#define QWSDLPARSERHANDLER_H_

#include <QSharedPointer>
#include <QXmlDefaultHandler>
#include <QXmlAttributes>

#include "Model/RequestResponseElement.h"
#include "Model/Type.h"

class QWSDLParserHandler : public QXmlDefaultHandler
{
public:
	QWSDLParserHandler();
	virtual ~QWSDLParserHandler();

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
	RequestResponseElementListSharedPtr getElementList() const;

private:
    QString m_szCurrentType;
    QString m_szCurrentSection;
    QString m_szCurrentText;

    QString m_szTargetNamespacePrefix;

    TypeListSharedPtr m_pListTypes;
    RequestResponseElementListSharedPtr m_pListElements;

    TypeSharedPtr m_pCurrentType;
    RequestResponseElementSharedPtr m_pCurrentElement;

};

#endif /* QWSDLPARSERHANDLER_H_ */
