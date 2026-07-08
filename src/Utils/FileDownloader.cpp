//
// Created by ebeuque on 08/07/2026.
//

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include "FileDownloader.h"

#include "Builder/FileHelper.h"

bool FileDownloader::downloadFile(const QString& szURL, const QString& szFilePath)
{
	bool bRes = true;

	QByteArray bytesFile;
	bRes = downloadFile(szURL, bytesFile);

	QFile file(szFilePath);
	if(file.open(QIODevice::WriteOnly)){
		file.write(bytesFile);
		file.close();
	}
	return bRes;
}

bool FileDownloader::downloadFile(const QString& szURL, QByteArray& bytesFile)
{
	bool bRes = true;

	// Download the file
	QNetworkAccessManager manager;
	QNetworkReply* reply = manager.get(QNetworkRequest(QUrl::fromUserInput(szURL)));
	QEventLoop loop;
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	// Handle HTTP 301: redirected url
	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if(!possibleRedirectUrl.isNull()){
		QUrl redirectedUrl = possibleRedirectUrl.toUrl();
		if(!redirectedUrl.isEmpty()){
			reply = manager.get(QNetworkRequest(redirectedUrl));
			QEventLoop loop;
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
			loop.exec();
		}
	}

	// Load response
	if (reply->error() == QNetworkReply::NoError) {
		bytesFile = reply->readAll();
	}else {
		qCritical("[File] Error downloading file: %s", qPrintable(reply->errorString()));
		bRes = false;
	}

	return bRes;
}