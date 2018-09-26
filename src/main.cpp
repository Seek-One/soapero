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

#include "Builder/FileBuilder.h"
#include "Builder/TypeListBuilder.h"
#include "Parser/QWSDLParserHandler.h"
#include "Utils/UniqueStringList.h"

QStringList getWSDLFileNames(const char* szPathSrc);
void copyPath(QString src, QString dst);
bool removeDir(const QString & dirName);

int main(int argc, char **argv)
{
	int iRes = 0;

	QCoreApplication::setApplicationName(APPLICATION_NAME);
	QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

	QCoreApplication a(argc, argv);

	if((argc < 3) || (argc > 4)){
		printf("Usage: ./jet1oeil-soapero SRC_DIR DST_DIR [FILE_TYPE] (where FILE_TYPE is \"Default\" or \"CMakeLists\")");
		return -1;
	}

	bool bFileGenerated = false;
	QSharedPointer<UniqueStringList> pListGeneratedFiles(new UniqueStringList());
	QStringList listWSDLFileNames = getWSDLFileNames(argv[1]);
	QStringList::const_iterator iter;
	for(iter = listWSDLFileNames.constBegin(); iter != listWSDLFileNames.constEnd(); ++iter){
		QFile file(QString(argv[1]) + QDir::separator() + *iter);
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
				bool bRes = true;
				if(!QDir(argv[2]).exists()) {
					qWarning("[main] Generated directory does not exists, we create it");
					bRes = QDir().mkdir(argv[2]);
				}

				if(bRes){
					TypeListBuilder builder(handler.getService(), handler.getTypeList(), handler.getRequestResponseElementList(), pListGeneratedFiles);
					builder.setNamespace("ONVIF");
					builder.setFilename("actionservice");
					builder.setDirname(argv[2]);
					builder.buildHeaderFiles();
					builder.buildCppFiles();

					bFileGenerated = true;

					QDir dir("./src/Base");
					foreach (QString f, dir.entryList(QDir::Files)) {
						QString srcPath = QString("./src/Base") + QDir::separator() + f;
						QString dstPath = QString(argv[2]) + QDir::separator() + "types" + QDir::separator() + f;
						QFile::remove(dstPath);
						QFile::copy(srcPath, dstPath);

						pListGeneratedFiles->append(QString("types") + QDir::separator() + f);
					}
					dir = QDir("./src/Service");
					foreach (QString f, dir.entryList(QDir::Files)) {
						QString srcPath = QString("./src/Service") + QDir::separator() + f;
						QString dstPath = QString(argv[2]) + QDir::separator() + f;
						QFile::remove(dstPath);
						QFile::copy(srcPath, dstPath);

						pListGeneratedFiles->append(f);
					}
				}else{
					qWarning("Failed to create directory \"%s\"", argv[2]);
				}
			}
		}else{
			qWarning("Error for opening file (%s)", qPrintable(file.errorString()));
		}
	}

	if(bFileGenerated){
		FileBuilder::FileType fileType = FileBuilder::Default;
		if(argc == 4){
			if(strcmp(argv[3], "CMakeLists") == 0){
				fileType = FileBuilder::CMakeLists;
			}
		}
		FileBuilder* pFileBuilder = FileBuilder::createFileBuilderFromType(fileType, "onvif", argv[2], pListGeneratedFiles);
		if(pFileBuilder){
			pFileBuilder->generateFile();
		}
	}

	return iRes;
}

QStringList getWSDLFileNames(const char* szPathSrc)
{
	QStringList listFileNames;
	QDir directory(szPathSrc);
	if(directory.exists()){
		listFileNames = directory.entryList(QStringList() << "*.wsdl" << "*.WSDL", QDir::Files);
	}
	return listFileNames;
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
