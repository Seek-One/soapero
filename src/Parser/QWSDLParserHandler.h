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

private:
    QString m_szCurrentSection;
    QString m_szCurrentText;

    TypeListSharedPtr m_pListTypes;

    TypeSharedPtr m_pCurrentType;

};

#endif /* QWSDLPARSERHANDLER_H_ */
