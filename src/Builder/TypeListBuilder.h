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

class TypeListBuilder
{
public:
	TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement);
	virtual ~TypeListBuilder();

	void setPrefix(const QString& szPrefix);
	QString getPrefix() const;

	void setFilename(const QString& szFilename);
	QString getFilename() const;

	void setDirname(const QString& szDirname);
	QString getDirname() const;

	void buildHeaderFile();
	void buildCppFile();


private:
	void buildHeaderClassType(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;

	void buildHeaderClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;

	void buildHeaderClassService(QTextStream& os, const ServiceSharedPtr& pService) const;

	void buildCppClassType(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildCppClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void buildCppClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;

	void buildCppClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;

	void buildCppClassService(QTextStream& os, const ServiceSharedPtr& pService) const;

	TypeListSharedPtr m_pListType;
	RequestResponseElementListSharedPtr m_pListElement;
	ServiceSharedPtr m_pService;

	QString m_szPrefix;
	QString m_szFilename;
	QString m_szDirname;
};

#endif /* TYPELISTBUILDER_H_ */
