//
// Created by ebeuque on 17/09/2021.
//

#ifndef COM_JET1OEIL_SOAPERO_FILEHELPER_H
#define COM_JET1OEIL_SOAPERO_FILEHELPER_H

#define WITH_DIR_CREATION

#include <QString>

class FileHelper
{
public:
	FileHelper();
	virtual ~FileHelper();

	static QString buildPath(const QString& szBaseDirectory, const QString& szFileNamespace, const QString& szFileCategory, const QString& szFileName);
	static bool createDirectoryForFile(const QString& szFilePath);

	static QString buildFileName(const QString& szFileNamespace, const QString& szBaseName, const QString& szExtension);

	static bool isFileTypes(const QString& szFilePath);
	static bool isFileMessage(const QString& szFilePath);
};


#endif //COM_JET1OEIL_SOAPERO_FILEHELPER_H
