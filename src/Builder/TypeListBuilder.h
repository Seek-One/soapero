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

#include "Model/Type.h"

class TypeListBuilder
{
public:
	TypeListBuilder(const TypeListSharedPtr& type);
	virtual ~TypeListBuilder();

	void setPrefix(const QString& szPrefix);
	QString getPrefix() const;

	void setFilename(const QString& szFilename);
	QString getFilename() const;

	void buildHeaderFile();


private:
	void buildHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const;
	void buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;

	TypeListSharedPtr m_pListType;

	QString m_szPrefix;
	QString m_szFilename;

};

#endif /* TYPELISTBUILDER_H_ */
