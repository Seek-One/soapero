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

	void build();

private:
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
