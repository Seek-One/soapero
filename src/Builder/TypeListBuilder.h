/*
 * TypeListBuilder.h
 *
 *  Created on: 17 juil. 2017
 *      Author: lgruber
 */

#ifndef TYPELISTBUILDER_H_
#define TYPELISTBUILDER_H_

#include <QFile>
#include <QTextStream>

#include "Model/RequestResponseElement.h"
#include "Model/ComplexType.h"
#include "Model/SimpleType.h"
#include "Model/Type.h"
#include "Model/Service.h"

#include "Utils/UniqueStringList.h"

class TypeListBuilder
{
public:
	enum FileCategory {
		FileCategory_Unknown,
		FileCategory_Type,
		FileCategory_Message,
		FileCategory_Service,
	};

public:
	TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement, const QSharedPointer<UniqueStringList>& pListGeneratedFiles);
	virtual ~TypeListBuilder();

	void setPrefix(const QString& szPrefix);
	QString getPrefix() const;

	void setNamespace(const QString& szNamespace);
	QString getNamespace() const;

	void setFilename(const QString& szFilename);
	QString getFilename() const;

	void setDirname(const QString& szDirname);
	QString getDirname() const;

	static QString getHeaderPath(const QString& szNamespace, const QString& szCategory, const QString& szFilename, FileCategory iOrigin);
	static QString getTypeHeaderPath(const TypeSharedPtr& pType, FileCategory iOrigin);
	static QString getSimpleTypeHeaderPath(const SimpleTypeSharedPtr& pSimpleType, FileCategory iOrigin);
	static QString getComplexTypeHeaderPath(const ComplexTypeSharedPtr& pComplexType, FileCategory iOrigin);
	static QString getRequestResponseElementHeaderPath(const RequestResponseElementSharedPtr& pRequestResponseElement, FileCategory iOrigin);

	static QString getDefine(const QString& szPrefix, const TypeSharedPtr& pType);
	static QString getDefine(const QString& szPrefix, const RequestResponseElementSharedPtr& pElement);
	static QString getDefine(const QString& szPrefix, const ServiceSharedPtr& pService);

	static QString getHeaderFileName(const TypeSharedPtr& pType);
	static QString getCppFileName(const TypeSharedPtr& pType);
	static QString getHeaderFileName(const SimpleTypeSharedPtr& pSimpleType);
	static QString getSimpleTypeNamespace(const SimpleTypeSharedPtr& pSimpleType);
	static QString getHeaderFileName(const RequestResponseElementSharedPtr& pElement);
	static QString getCppFileName(const RequestResponseElementSharedPtr& pElement);

	void buildHeaderFiles();
	void buildCppFiles();

private:
	void buildFileDescription(QTextStream& os, const QString& szFilename);

	void buildHeaderFile(const TypeSharedPtr& pType);
	void buildHeaderFile(const RequestResponseElementSharedPtr& pElement);
	void buildHeaderFile(const ServiceSharedPtr& pService);
	void buildHeaderFileDescription(QTextStream& os, const QString& szFilename, const QString& szDefine);

	void buildCppFile(const TypeSharedPtr& pType);
	void buildCppFile(const RequestResponseElementSharedPtr& pElement);
	void buildCppFile(const ServiceSharedPtr& pService);
	void buildCppFileDescription(QTextStream& os, const QString& szFilename);

	void buildHeaderClassType(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void buildHeaderClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;
	void buildHeaderClassService(QTextStream& os, const ServiceSharedPtr& pService) const;
	void buildHeaderIncludeType(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildHeaderIncludeElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;
	void buildHeaderIncludeService(QTextStream& os, const ServiceSharedPtr& pService) const;

	void buildCppClassType(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildCppClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void buildCppClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace = QString()) const;
	void buildCppClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;
	void buildCppClassService(QTextStream& os, const ServiceSharedPtr& pService) const;

	void startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const;
	void endCppClass(QTextStream& os) const;

	void buildTypeIncludes(QTextStream& os, const TypeSharedPtr& pType);

	TypeListSharedPtr m_pListType;
	RequestResponseElementListSharedPtr m_pListElement;
	ServiceSharedPtr m_pService;

	QSharedPointer<UniqueStringList> m_pListGeneratedFiles;

	QString m_szPrefix;
	QString m_szNamespace;
	QString m_szFilename;
	QString m_szDirname;
};

#endif /* TYPELISTBUILDER_H_ */
