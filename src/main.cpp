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
#include <QDir>
#include <QCoreApplication>

#include "Loader/QWSDLLoader.h"
#include "Loader/QServicesLoader.h"

#include "Builder/FileBuilder.h"

#include "Utils/UniqueStringList.h"

void copyPath(QString src, QString dst);
bool removeDir(const QString & dirName);


int main(int argc, char **argv)
{
	int iRes = 0;

	QCoreApplication::setApplicationName(APPLICATION_NAME);
	QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

	QCoreApplication a(argc, argv);

	bool bShowHelp = false;

	if(argc < 2){
		bShowHelp = true;
	}

	QString szOutputMode = "Default";
	QString szNamespace;
	QString szResourcePath = "./resources";
	QString szServiceName;

	// Parse extra args
	for(int i=3; i<argc; i++)
	{
		QString szArg = argv[i];
		if(szArg.startsWith("--output-mode=")){
			szOutputMode = szArg.mid(14);
		}
		if(szArg.startsWith("--namespace=")){
			szNamespace = szArg.mid(12);
		}
		if(szArg.startsWith("--resources-dir=")){
			szResourcePath = szArg.mid(16);
		}
		if(szArg.startsWith("--service-name=")){
			szServiceName = szArg.mid((15));
		}
	}

	if(szNamespace.isEmpty()){
		bShowHelp = true;
	}

	if(bShowHelp){
		printf("Usage: ./jet1oeil-soapero SRC_DIR DST_DIR\r\n");
		printf("       --namespace=[NAMESPACE]: Global namespace to use for generated class (Mandatory)\r\n");
		printf("       --output-mode=[OUTPUT_MODE]: \"Default\" or \"CMakeLists\"\r\n");
		printf("       --resources-dir=[DIR]: Path where to find resources files. Used to copy some files in output directory. (Default: ./resources)\r\n");
		printf("       --service-name=[SERVICE_NAME]: Force service name, useful for wsdl service not defining the name tag.\r\n");
		return -1;
	}

	const char* szWSDLFilesDirectory = argv[1];

	QString szOutputDirectory = argv[2];

	bool bFileGenerated = false;

	bool bGoOn = true;

	// Loading list of WSDL files
	QSharedPointer<UniqueStringList> pListGeneratedFiles(new UniqueStringList());

	QServicesLoader servicesLoader;
	servicesLoader.setSourceDirectory(szWSDLFilesDirectory);
	servicesLoader.setServiceName(szServiceName);
	servicesLoader.setNamespace(szNamespace);
	servicesLoader.setResourcePath(szResourcePath);
	servicesLoader.setOutputDirectory(szOutputDirectory);
	bGoOn = servicesLoader.load(bFileGenerated, pListGeneratedFiles);

	if(bGoOn && bFileGenerated){
		FileBuilder::FileType fileType = FileBuilder::Default;
		if(szOutputMode == "CMakeLists"){
			fileType = FileBuilder::CMakeLists;
		}
		FileBuilder* pFileBuilder = FileBuilder::createFileBuilderFromType(fileType, szNamespace, szOutputDirectory, pListGeneratedFiles);
		if(pFileBuilder){
			pFileBuilder->generateFile();
		}
	}

	const auto& iSuccessCount = servicesLoader.getSuccessCount();
	const auto& iWSDLFilesCount = servicesLoader.getWSDLFilesCount();
	qDebug("[Main] %d/%d WSDL files have been processed successfully '%s'", iSuccessCount, iWSDLFilesCount, szWSDLFilesDirectory);

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
