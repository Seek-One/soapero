//
// Created by ebeuque on 08/07/2026.
//

#ifndef SOAPERO_FILEDOWNLOADER_H
#define SOAPERO_FILEDOWNLOADER_H

#include <QString>

class FileDownloader
{
public:
	static bool downloadFile(const QString& szURL, const QString& szFilePath);
	static bool downloadFile(const QString& szURL, QByteArray& bytesFile);
};


#endif //SOAPERO_FILEDOWNLOADER_H