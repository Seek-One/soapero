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
#include <QCoreApplication>

#include "Builder/FileBuilder.h"
#include "Builder/TypeListBuilder.h"
#include "Builder/FileHelper.h"

#include "Parser/QWSDLParser.h"
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

	bool bShowHelp = false;

	if(argc < 2){
		bShowHelp = true;
	}

	QString szOutputMode = "Default";
	QString szNamespace;
	QString szResourcePath = "./resources";

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
	}

	if(szNamespace.isEmpty()){
		bShowHelp = true;
	}

	if(bShowHelp){
		printf("Usage: ./jet1oeil-soapero SRC_DIR DST_DIR\r\n");
		printf("       --namespace=[NAMESPACE]: Global namespace to use for generated class (Mandatory)\r\n");
		printf("       --output-mode=[OUTPUT_MODE]: \"Default\" or \"CMakeLists\"\r\n");
		printf("       --resources-dir=[DIR]: Path where to find resources files. Used to copy some files in output directory. (Default: ./resources)\r\n");
		return -1;
	}

	const char* szWSDLFilesDirectory = argv[1];
	QDir dirWDSLFiles(szWSDLFilesDirectory);

	QString szOutputDirectory = argv[2];

	bool bFileGenerated = false;

	bool bGoOn;

	// Loading list of WSDL files
	QSharedPointer<UniqueStringList> pListGeneratedFiles(new UniqueStringList());
	QStringList listWSDLFileNames = getWSDLFileNames(szWSDLFilesDirectory);
	qDebug("[Main] %d WSDL files have been found in directory '%s'", listWSDLFileNames.count(), szWSDLFilesDirectory);

	// Iterate over each WSDL files
	QStringList::const_iterator iter;
	for(iter = listWSDLFileNames.constBegin(); iter != listWSDLFileNames.constEnd(); ++iter)
	{
		// Open current WSDL file
		QString szFilename = *iter;
		QString szFilePath = dirWDSLFiles.filePath(szFilename);
		qDebug("[Main] Processing file '%s'", qPrintable(szFilename));

		QFile file(szFilePath);
		if(file.open(QFile::ReadOnly)) {
			bGoOn = true;

			// Read all file
			QByteArray bytes = file.readAll();
			QBuffer buffer;
			if(bytes.size() != 0){
				buffer.setData(bytes);
			}else{
				bGoOn = false;
				qWarning("[Main] File has no data");
			}

			// Parse WSDL file in XML format
			QWSDLParser parser;
			if(bGoOn){
				QXmlStreamReader xmlReader;
				xmlReader.addData(bytes);
				bGoOn = parser.parse(xmlReader);
				if(!bGoOn){
					qWarning("[Main] Error to parse data (error: %s)", qPrintable(xmlReader.errorString()));
				}
			}
			
			// Create directory output if not existing
			if(bGoOn){
				if(!QDir(szOutputDirectory).exists()) {
					qDebug("[Main] Creating non-existing directory '%s'", qPrintable(szOutputDirectory));
					bGoOn = QDir().mkdir(szOutputDirectory);
				}
			}

			// Build file for service
			if(bGoOn){
				TypeListBuilder builder(parser.getService(), parser.getTypeList(), parser.getRequestResponseElementList(), pListGeneratedFiles);
				builder.setNamespace(szNamespace);
				builder.setFilename("actionservice");
				builder.setDirname(szOutputDirectory);
				builder.buildHeaderFiles();
				builder.buildCppFiles();

				bFileGenerated = true;
			}

			// Add class for base type
			if(bGoOn){
				QString szResourcesBasePath = QDir(szResourcePath).filePath("Base");
				QDir dir(szResourcesBasePath);
				if(dir.exists()){
					foreach (QString f, dir.entryList(QDir::Files))
					{
						QString szSrcPath = QDir(szResourcesBasePath).filePath(f);
						QString szDstFullPath = FileHelper::buildPath(szOutputDirectory, "xs", "types", f);
						QString szDstShortPath = FileHelper::buildPath(QString(), "xs", "types", f);

						// Remove the path
						QFile::remove(szDstFullPath);

						// Create directory for file
						FileHelper::createDirectoryForFile(szDstFullPath);

						// Copy the path
						QFile::copy(szSrcPath, szDstFullPath);

						pListGeneratedFiles->append(szDstShortPath);
					}
				}else{
					qDebug("[Main] Base files directory not found %s", qPrintable(szResourcesBasePath));
				}
			}

			if(bGoOn){
				QString szResourcesServicePath = QDir(szResourcePath).filePath("Service");
				QDir dir = QDir(szResourcesServicePath);
				if(dir.exists()){
					// Copy services files
					foreach (QString f, dir.entryList(QDir::Files))
					{
						QString szSrcPath = QDir(szResourcesServicePath).filePath(f);
						QString szDstPath = QDir(szOutputDirectory).filePath(f);

						// Remove old file
						QFile::remove(szDstPath);

						bool bCopyOnly = true;
						if(f.endsWith(".h") || f.endsWith(".cpp")){
							bCopyOnly = false;
						}

						if(bCopyOnly){
							// Copy the file
							QFile::copy(szSrcPath, szDstPath);
						}else{
							// Update namespace
							QByteArray fileData;
							QFile fileSrc(szSrcPath);
							bGoOn = fileSrc.open(QIODevice::ReadOnly);
							if(bGoOn){
								// Load content and replace namespace
								fileData = fileSrc.readAll();
								QString text(fileData);
								text.replace(QString("namespace SOAPERO"), QString("namespace %0").arg(szNamespace));

								QFile fileDst(szDstPath);
								bGoOn = fileDst.open(QIODevice::ReadWrite);
								if(bGoOn){
									fileDst.write(text.toUtf8());
									fileDst.close();
								}
								fileSrc.close();
							}
						}

						file.close(); // close the file handle.


						pListGeneratedFiles->append(f);
					}
				}else{
					qDebug("[Main] Service files directory not found %s", qPrintable(szResourcesServicePath));
				}
			}else{
				qWarning("[Main] Failed to create directory '%s'", qPrintable(szOutputDirectory));
			}
		}else{
			qWarning("[Main] Error for opening file %s", qPrintable(file.errorString()));
		}
	}

	if(bFileGenerated){
		FileBuilder::FileType fileType = FileBuilder::Default;
		if(szOutputMode == "CMakeLists"){
			fileType = FileBuilder::CMakeLists;
		}
		FileBuilder* pFileBuilder = FileBuilder::createFileBuilderFromType(fileType, szNamespace, szOutputDirectory, pListGeneratedFiles);
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
		listFileNames = directory.entryList(QStringList() << "*.wsdl" << "*.WSDL" << "*.xsd" << "*.XSD", QDir::Files);
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
