/*
 * FileBuilder.cpp
 *
 *  Created on: 3 sept. 2018
 *      Author: alavier
 */

#include <QDir>
#include <QTextStream>

#include "FileBuilder.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

class DefaultFileBuilder : public FileBuilder
{
public:
	DefaultFileBuilder(const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
		: FileBuilder(szDirName, pFileList)
	{

	}

	virtual ~DefaultFileBuilder()
	{

	}

	void generateFile() const
	{
		QDir dir(m_szDirName);
		QFile file(m_szDirName + QDir::separator() + "resume.txt");
		if(file.open(QFile::WriteOnly)){
			QTextStream os(&file);

			os << "Generated file list:" << CRLF << CRLF;

			QList<QString>::const_iterator iter;
			for(iter = m_pFileList->constBegin(); iter != m_pFileList->constEnd(); ++iter){
				os << *iter << CRLF;
			}
		}
	}
};

bool stringLessThan(const QString& sz1, const QString& sz2)
{
	return sz2 < sz2;
}

FileBuilder::FileBuilder(const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
	: m_szDirName(szDirName), m_pFileList(pFileList)
{
	m_pFileList->sort();
}

FileBuilder::~FileBuilder()
{

}

FileBuilder* FileBuilder::createFileBuilderFromType(FileType fileType, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
{
	switch(fileType){
	default:
		return new DefaultFileBuilder(szDirName, pFileList);
	}
}
