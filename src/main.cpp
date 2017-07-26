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
#include <QBuffer>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QCoreApplication>

#include "Builder/TypeListBuilder.h"
#include "Parser/QWSDLParserHandler.h"

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


			TypeListBuilder builder(handler.getService(), handler.getTypeList(), handler.getElementList());
			//builder.setPrefix("Onvif");
			builder.setFilename("doorcontrol");
			builder.buildHeaderFile();
			builder.buildCppFile();

		}
	}else{
		qWarning("Error for opening file (%s)", qPrintable(file.errorString()));
	}


	return iRes;
}
