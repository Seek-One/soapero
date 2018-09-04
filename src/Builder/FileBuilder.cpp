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
	DefaultFileBuilder(const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
		: FileBuilder(szName, szDirName, pFileList)
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

class CMakeListsFileBuilder : public FileBuilder
{
public:
	CMakeListsFileBuilder(const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
		: FileBuilder(szName, szDirName, pFileList)
	{

	}

	virtual ~CMakeListsFileBuilder()
	{

	}

	void generateFile() const
	{
		QDir dir(m_szDirName);
		QFile file(m_szDirName + QDir::separator() + "CMakeLists.txt");
		if(file.open(QFile::WriteOnly)){
			QList<QString> listTypes;
			QList<QString> listMessages;
			QList<QString> listServices;
			QList<QString>::const_iterator iter;
			for(iter = m_pFileList->constBegin(); iter != m_pFileList->constEnd(); ++iter){
				if(iter->startsWith(QString("types") + QDir::separator())){
					listTypes.append(*iter);
				}else if(iter->startsWith(QString("messages") + QDir::separator())){
					listMessages.append(*iter);
				}else{
					listServices.append(*iter);
				}
			}

			QTextStream os(&file);
			os << "SET(TYPES_SRC" << CRLF;
			Q_FOREACH(QString szType, listTypes){
				os << "\t" << szType << CRLF;
			}
			os << ")" << CRLF << CRLF;

			os << "SET(MESSAGES_SRC" << CRLF;
			Q_FOREACH(QString szMessage, listMessages){
				os << "\t" << szMessage << CRLF;
			}
			os << ")" << CRLF << CRLF;

			os << "SET(SERVICES_SRC" << CRLF;
			Q_FOREACH(QString szService, listServices){
				os << "\t" << szService << CRLF;
			}
			os << ")" << CRLF << CRLF;

			os << "SET(" << m_szName.toUpper() << "_SRC" << CRLF;
			os << "\t" << "${TYPES_SRC}" << CRLF;
			os << "\t" << "${MESSAGES_SRC}" << CRLF;
			os << "\t" << "${SERVICES_SRC}" << CRLF;
			os << ")" << CRLF << CRLF;

			os << "add_library (" << m_szName << " ${" << m_szName.toUpper() << "_SRC})" << CRLF << CRLF;

			os << "if(WITH_INSTALL_LIB)" << CRLF;
			os << "\tinstall(TARGETS " << m_szName << " DESTINATION ${INSTALL_PATH_LIB})" << CRLF;
			os << "endif()" << CRLF << CRLF;
		}
	}
};

bool stringLessThan(const QString& sz1, const QString& sz2)
{
	return sz2 < sz2;
}

FileBuilder::FileBuilder(const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
	: m_szName(szName), m_szDirName(szDirName), m_pFileList(pFileList)
{
	m_pFileList->sort();
}

FileBuilder::~FileBuilder()
{

}

FileBuilder* FileBuilder::createFileBuilderFromType(FileType fileType, const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList)
{
	switch(fileType){
	case CMakeLists:
		return new CMakeListsFileBuilder(szName, szDirName, pFileList);
	default:
		return new DefaultFileBuilder(szName, szDirName, pFileList);
	}
}
