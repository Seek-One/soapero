/*
 * main.cpp
 *
 *  Created on: 12 juil. 2017
 *      Author: lgruber
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_VERSION_H
#include <version.h>
#endif

#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QCoreApplication>

#include "Builder/TypeListBuilder.h"
#include "Parser/QWSDLParserHandler.h"

void copyPath(QString src, QString dst);

int main(int argc, char **argv)
{
	int iRes = 0;

	QCoreApplication::setApplicationName(APPLICATION_NAME);
	QCoreApplication::setApplicationVersion(APPLICATION_VERSION);


	QFile file("./doorcontrol.wsdl");

	if(file.open(QFile::ReadOnly)) {

		QByteArray bytes = file.readAll();
		QBuffer buffer;
		buffer.setData(bytes);

		QXmlInputSource source(&buffer);
		QXmlSimpleReader reader;
		QWSDLParserHandler handler;
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);
		reader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
		reader.setFeature("http://xml.org/sax/features/namespaces", true);
		if(!reader.parse(source)){
			qWarning("[ServerController::getDevicesList] Error to parse data (error: %s)", qPrintable(reader.errorHandler()->errorString()));
		}else{

			if(!QDir("./generated").exists()) {
				qWarning("[main] Generated directory does not exists, we create it");
				QDir().mkdir("generated");
			}
			if(!QDir("./generated/xstypes").exists()) {
				qWarning("[main] XSTypes directory does not exists, we create it");
				QDir().mkdir("generated/xstypes");
			}


			TypeListBuilder builder(handler.getService(), handler.getTypeList(), handler.getElementList());
			//builder.setPrefix("Onvif");
			builder.setFilename("doorcontrol");
			builder.setDirname("./generated");
			builder.buildHeaderFile();
			builder.buildCppFile();

			QDir dir("./src/Base");
		    foreach (QString f, dir.entryList(QDir::Files)) {
		    	QString srcPath = QString("./src/Base") + QDir::separator() + f;
		    	QString dstPath = QString("./generated/xstypes") + QDir::separator() + f;
		    	QFile::remove(dstPath);
		        QFile::copy(srcPath, dstPath);
		    }

		}
	}else{
		qWarning("Error for opening file (%s)", qPrintable(file.errorString()));
	}


	return iRes;
}

void copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}
