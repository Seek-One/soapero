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
bool removeDir(const QString & dirName);

int main(int argc, char **argv)
{
	int iRes = 0;

	QCoreApplication::setApplicationName(APPLICATION_NAME);
	QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

	QCoreApplication a(argc, argv);

//	QFile file("./wsdl/ONVIF/doorcontrol.wsdl");			// WORKS!
//	QFile file("./wsdl/t-1.xsd");							// WORKS!
//	QFile file("./wsdl/ws-addr.xsd");						// WORKS!
//	QFile file("./wsdl/bf-2.xsd");							// WORKS!
//	QFile file("./wsdl/b-2.xsd");							// WORKS!
//	QFile file("./wsdl/xmlmime.xml");						// WORKS!
//	QFile file("./wsdl/soap-envelope.xml");					// WORKS!
//	QFile file("./wsdl/ONVIF/common.xsd");					// WORKS!
//	QFile file("./wsdl/include.xml");						// WORKS!
//	QFile file("./wsdl/ONVIF/onvif.xsd");					// WORKS!
	QFile file("./wsdl/ONVIF/devicemgmt.wsdl");

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

			//removeDir("./generated");

			if(!QDir("./generated").exists()) {
				qWarning("[main] Generated directory does not exists, we create it");
				QDir().mkdir("generated");
			}
			if(!QDir("./generated/types").exists()) {
				qWarning("[main] types directory does not exists, we create it");
				QDir().mkdir("generated/types");
			}


			TypeListBuilder builder(handler.getService(), handler.getTypeList(), handler.getRequestResponseElementList());
			builder.setNamespace("ONVIF");
			builder.setFilename("actionservice");
			builder.setDirname("./generated");
			builder.buildHeaderFiles();
			builder.buildCppFiles();
			builder.buildResumeFile();

			QDir dir("./src/Base");
		    foreach (QString f, dir.entryList(QDir::Files)) {
		    	QString srcPath = QString("./src/Base") + QDir::separator() + f;
		    	QString dstPath = QString("./generated/types") + QDir::separator() + f;
		    	QFile::remove(dstPath);
		        QFile::copy(srcPath, dstPath);
		    }
			dir = QDir("./src/Service");
		    foreach (QString f, dir.entryList(QDir::Files)) {
		    	QString srcPath = QString("./src/Service") + QDir::separator() + f;
		    	QString dstPath = QString("./generated") + QDir::separator() + f;
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

bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}
