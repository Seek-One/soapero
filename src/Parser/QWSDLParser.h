/*
 * QWSDLParserHandler.h
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#ifndef QWSDLPARSER_H_
#define QWSDLPARSER_H_

#include <QSharedPointer>
#include <QStack>
#include <QXmlStreamReader>

#include "Model/RequestResponseElement.h"
#include "Model/Message.h"
#include "Model/Operation.h"
#include "Model/Service.h"
#include "Model/Type.h"

#include "Parser/WSDLSections.h"
#include "Parser/QWSDLData.h"

class QWSDLParser
{
public:
	QWSDLParser();
	virtual ~QWSDLParser();

	void initXMLAttributes();		// https://www.w3.org/2001/xml.xsd
	void setLogIndent(int iIdent);

	void setWSDLData(const QSharedPointer<QWSDLData>& pDataLoaded);

	void setInitialNamespaceUri(const QString& szNamespaceUri);

	bool parse(QXmlStreamReader& xmlReader);

	TypeListSharedPtr getTypeList() const;
	RequestResponseElementListSharedPtr getRequestResponseElementList() const;
	ServiceSharedPtr getService() const;
	AttributeListSharedPtr getAttributeList() const;
	ElementListSharedPtr getElementList() const;

private:
	bool endDocument();

	bool readXMLNamespaces(QXmlStreamReader& xmlReader);

	///////////////////////////////////////////////////////////////
	// WSDL
	// http://schemas.xmlsoap.org/wsdl/
	///////////////////////////////////////////////////////////////
	bool readDefinitions(QXmlStreamReader& xmlReader);
	bool readTypes(QXmlStreamReader& xmlReader);
	bool readMessage(QXmlStreamReader& xmlReader);
	bool readPortType(QXmlStreamReader& xmlReader);
	bool readPart(QXmlStreamReader& xmlReader);
	bool readOperation(QXmlStreamReader& xmlReader);
	bool readInput(QXmlStreamReader& xmlReader);
	bool readOutput(QXmlStreamReader& xmlReader);

	///////////////////////////////////////////////////////////////
	// XML Schema Part 1: Structures
	// https://www.w3.org/2009/XMLSchema/XMLSchema.dtd
	///////////////////////////////////////////////////////////////

	// Schema
	bool readSchema(QXmlStreamReader& xmlReader);

	bool readComplexType(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readComplexContent(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readSimpleContent(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readExtension(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readElement(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool isGroup(const QString& szTagName) const;
	bool readGroup(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection);
	bool readSequence(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readAny(QXmlStreamReader& xmlReader);
	bool isAttribute(const QString& szTagName) const;
	bool readAttribute(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool isAttributeGroup(const QString& szTagName) const;
	bool readAttributeGroup(QXmlStreamReader& xmlReader, Section::Name iParentSection);

	// Composition
	bool isComposition(const QString& szTagName) const;
	bool readComposition(QXmlStreamReader& xmlReader, const QString& szTagName);
	bool readInclude(QXmlStreamReader& xmlReader);
	bool readImport(QXmlStreamReader& xmlReader);

	// Mgs
	bool isMgs(const QString& szTagName) const;
	bool readMsg(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection);

	// attrDecls
	bool isAttrDecls(const QString& szTagName) const;
	bool readAttrDecls(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection);

	// particleAndAttrs
	bool isParticleAndAttrs(const QString& szTagName) const;
	bool readParticleAndAttrs(QXmlStreamReader& xmlReader, const QString& szTagName, Section::Name iParentSection);

	///////////////////////////////////////////////////
	// XML Schemas Part 2: Datatypes
	// https://www.w3.org/2009/XMLSchema/datatypes.dtd
	///////////////////////////////////////////////////

	// Types
	bool readSimpleType(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readRestriction(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readList(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readUnion(QXmlStreamReader& xmlReader, Section::Name iParentSection);
	bool readMaxLength(QXmlStreamReader& xmlReader);
	bool readMinLength(QXmlStreamReader& xmlReader);
	bool readEnumeration(QXmlStreamReader& xmlReader);
	bool isFacet(const QString& szTagName);
	bool readFacet(QXmlStreamReader& xmlReader, const QString& szTagName);

private:
	ElementSharedPtr getElementByRef(const QString& szRef);
	TypeSharedPtr getTypeByName(const QString& szLocalName, const QString& szNamespace = QString(), const TypeListSharedPtr& pListIgnoredTypes = TypeList::create());
	TypeRefSharedPtr getTypeRefByTypeName(const QString& szTypeName, const QString& szNamespace = QString());

	bool isWSDLSchema(const QString& szQName);

	// Remote file loading
	bool loadFromHttp(const QString& szURL, const QString& szNamespace = QString());
	bool loadFromFile(const QString& szFileName, const QString& szNamespace = QString());

	// Current type to build
	void pushCurrentType(const TypeSharedPtr& pCurrentType);
	void popCurrentType();
	const TypeSharedPtr getCurrentType() const;

	// Current target namespace
	bool pushCurrentTargetNamespace(const QString& szTargetNamespaceURI);
	void popCurrentTargetNamespace();
	void updateTargetNamespacePrefix(const QString& szTargetNamespaceURI);

	// Logger functions
	void logParser(const QString& szMsg);
	void incrLogIndent();
	void decrLogIndent();

private:
    QString m_szCurrentType;
    QString m_szCurrentText;
    QString m_szCurrentOperationName;

    QSharedPointer<QWSDLData> m_pWSDLData;

    // Current namespace
    QString m_szCurrentTargetNamespacePrefix;
    QString m_szCurrentTargetNamespaceUri;
    QString m_szCurrentSchemaNamespacePrefix;

    bool m_bWaitForSoapEnvelopeFault;

    TypeListSharedPtr m_pListTypes;
    RequestResponseElementListSharedPtr m_pListRequestResponseElements;
    MessageListSharedPtr m_pListMessages;
    OperationListSharedPtr m_pListOperations;
    AttributeListSharedPtr m_pListAttributes;
    ElementListSharedPtr m_pListElements;

    ServiceSharedPtr m_pService;

    QStack<Section::Name> m_stackSection;
    QStack<TypeSharedPtr> m_stackCurrentTypes;
    QStack<QString> m_stackTargetNamespace;
    RequestResponseElementSharedPtr m_pCurrentRequestResponseElement;
    MessageSharedPtr m_pCurrentMessage;
    OperationSharedPtr m_pCurrentOperation;
    AttributeSharedPtr m_pCurrentAttribute;
    ElementSharedPtr m_pCurrentElement;


    int m_iLogIndent;
};

#endif /* QWSDLPARSER_H_ */
