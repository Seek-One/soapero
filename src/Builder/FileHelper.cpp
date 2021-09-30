//
// Created by ebeuque on 17/09/2021.
//

#include <QDir>

#include "FileHelper.h"

FileHelper::FileHelper()
{

}

FileHelper::~FileHelper()
{

}

bool FileHelper::isFileTypes(const QString& szFilePath)
{
#ifdef WITH_DIR_CREATION
	return szFilePath.contains("/types/");
#else
	return szFilePath.startsWith("types/") || szFilePath.startsWith("/types/");
#endif
}

bool FileHelper::isFileMessage(const QString& szFilePath)
{
#ifdef WITH_DIR_CREATION
	return szFilePath.contains("/messages/");
#else
	return szFilePath.startsWith("messages/") || szFilePath.startsWith("/messages/");
#endif
}

QString FileHelper::buildPath(const QString& szBaseDirectory, const QString& szFileNamespace, const QString& szFileCategory, const QString& szFileName)
{
	QDir dir;

	if(!szBaseDirectory.isEmpty()){
		dir.setPath(szBaseDirectory);
	}

#ifdef WITH_DIR_CREATION
	if(!szFileNamespace.isEmpty()){
		dir.setPath(dir.filePath(szFileNamespace.toLower()));
	}

	if(!szFileCategory.isEmpty()){
		dir.setPath(dir.filePath(szFileCategory));
	}

#else
	if(!szFileCategory.isEmpty()){
		dir.setPath(dir.filePath(szFileCategory));
	}
#endif
	return dir.filePath(szFileName);
}

bool FileHelper::createDirectoryForFile(const QString& szFilePath)
{
	bool bRes = true;

	QFileInfo fileInfo(szFilePath);
	QDir dirPath = fileInfo.dir();
	if(!dirPath.exists()){
		bRes = dirPath.mkpath(".");
	}

	return bRes;
}

QString FileHelper::buildFileName(const QString& szFileNamespace, const QString& szBaseName, const QString& szExtension)
{
	QString szFileName;

#ifdef WITH_DIR_CREATION
	// Mingw32 don't support the have the project with multiple same filename
	if(!szFileNamespace.isEmpty()){
		szFileName += (szFileNamespace.toUpper() + "-");
	}
#endif
	szFileName += szBaseName;
	szFileName += "." + szExtension;

	return szFileName;
}