/*
 * FileBuilder.h
 *
 *  Created on: 3 sept. 2018
 *      Author: alavier
 */

#ifndef SRC_BUILDER_FILEBUILDER_H_
#define SRC_BUILDER_FILEBUILDER_H_

#include <QList>
#include <QSharedPointer>
#include <QString>

class FileBuilder {
public:
	enum FileType {
		Default,
		CMakeLists
	};

protected:
	FileBuilder(const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList);

public:
	static FileBuilder* createFileBuilderFromType(FileType fileType, const QString& szName, const QString& szDirName, const QSharedPointer<QList<QString> >& pFileList);

	virtual ~FileBuilder();

	virtual void generateFile() const = 0;

protected:
	QString m_szName;
	QString m_szDirName;
	QSharedPointer<QList<QString> > m_pFileList;
};

#endif /* SRC_BUILDER_FILEBUILDER_H_ */
