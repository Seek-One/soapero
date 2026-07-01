/*
 * TypeListBuilder.cpp
 *
 *  Created on: 17 juil. 2017
 *      Author: lgruber
 */

#include <QDateTime>
#include <QCoreApplication>
#include <QStringList>
#include <QDir>

#include "../Utils/StringUtils.h"
#include "TypeListBuilder.h"
#include "FileHelper.h"

#include "TargetEngine/QtCppTargetEngine.h"

#include "../common.h"

TypeListBuilder::TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement, const QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	m_pListType = pListType;
	m_pListElement = pListElement;
	m_pService = pService;

	m_pListGeneratedFiles = pListGeneratedFiles;

	m_szPrefix = "";
}

TypeListBuilder::~TypeListBuilder()
{

}

void TypeListBuilder::setPrefix(const QString& szPrefix)
{
	m_szPrefix = szPrefix;
}

QString TypeListBuilder::getPrefix() const
{
	return m_szPrefix;
}

void TypeListBuilder::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

QString TypeListBuilder::getNamespace() const
{
	return m_szNamespace;
}

void TypeListBuilder::setFilename(const QString& szFilename)
{
	m_szFilename = szFilename;
}

QString TypeListBuilder::getFilename() const
{
	return m_szFilename;
}

void TypeListBuilder::setDirname(const QString& szDirname)
{
	m_szDirname = szDirname;
}

QString TypeListBuilder::getDirname() const
{
	return m_szDirname;
}

void TypeListBuilder::build()
{
	QtCppTargetEngine targetEngine;
	targetEngine.setOutputDirectory(m_szDirname);
	targetEngine.setNamespace(m_szNamespace);
	targetEngine.setPrefix(m_szPrefix);
	targetEngine.setService(m_pService);
	targetEngine.setRequestResponseElementList(m_pListElement);
	targetEngine.setTypeList(m_pListType);
	targetEngine.setGeneratedFileList(m_pListGeneratedFiles);
	targetEngine.build();
}
