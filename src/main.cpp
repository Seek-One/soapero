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

bool processWSDLFile(const QString& szFilename, const QString& szFilePath, const QString& szParamServiceName, const QString& szOutputDirectory, const QString& szResourcePath, const QString& szNamespace, bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	bool bRes;

	QString szServiceName;

	// Open current WSDL file
	qDebug("[Main] Processing file '%s'", qPrintable(szFilename));

	QFile file(szFilePath);
	bRes = file.open(QFile::ReadOnly);
	if(bRes) {
		// Read all file
		QByteArray bytes = file.readAll();
		QBuffer buffer;
		if(bytes.size() != 0){
			buffer.setData(bytes);
		}else{
			bRes = false;
			qWarning("[Main] File has no data");
		}

		// Parse WSDL file in XML format
		QWSDLParser parser;
		if(bRes){
			QXmlStreamReader xmlReader;
			xmlReader.addData(bytes);
			bRes = parser.parse(xmlReader);
			if(!bRes){
				qWarning("[Main] Error to parse data (error: %s)", qPrintable(xmlReader.errorString()));
			}
		}

		// Check service
		ServiceSharedPtr pService;
		QString szCurrentServiceName;
		if(bRes){
			pService = parser.getService();
			if(szParamServiceName.isNull()){
				szCurrentServiceName = pService->getName();
			}else{
				szCurrentServiceName = szParamServiceName;
				pService->setName(szServiceName);
			}
			if(szCurrentServiceName.isNull()){
				qDebug("[Main] Service name is not defined");
				bRes = false;
			}
		}

		// Create directory output if not existing
		if(bRes){
			if(!QDir(szOutputDirectory).exists()) {
				qDebug("[Main] Creating non-existing directory '%s'", qPrintable(szOutputDirectory));
				bRes = QDir().mkdir(szOutputDirectory);
			}
		}

		// Build file for service
		if(bRes){
			const auto& pRequestResponseElementList = parser.getRequestResponseElementList();
			const auto& pServiceTypeList = parser.getTypeList();
			qDebug("[Main] Build files for service: %s (elements: %d, types: %d)", qPrintable(pService->getName()), (int)pRequestResponseElementList->count(), (int)pServiceTypeList->count());
			//pRequestResponseElementList->print();
			//pServiceTypeList->print();

			TypeListBuilder builder(pService, pServiceTypeList, parser.getRequestResponseElementList(), pListGeneratedFiles);
			builder.setNamespace(szNamespace);
			builder.setFilename("actionservice");
			builder.setDirname(szOutputDirectory);
			builder.build();

			bFileGenerated = true;
		}

		// Add class for base type
		if(bRes){
			QString szResourcesBasePath = QDir(szResourcePath).filePath("Base/xs/types");
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

		if(bRes){
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
						bRes = fileSrc.open(QIODevice::ReadOnly);
						if(bRes){
							// Load content and replace namespace
							fileData = fileSrc.readAll();
							QString text(fileData);
							text.replace(QString("namespace SOAPERO"), QString("namespace %0").arg(szNamespace));

							QFile fileDst(szDstPath);
							bRes = fileDst.open(QIODevice::ReadWrite);
							if(bRes){
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
		}
	}else{
		qWarning("[Main] Error for opening file %s", qPrintable(file.errorString()));
	}

	return bRes;
}

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
	QDir dirWDSLFiles(szWSDLFilesDirectory);

	QString szOutputDirectory = argv[2];

	bool bFileGenerated = false;

	bool bGoOn = true;

	// Loading list of WSDL files
	QSharedPointer<UniqueStringList> pListGeneratedFiles(new UniqueStringList());
	QStringList listWSDLFileNames = getWSDLFileNames(szWSDLFilesDirectory);
	qDebug("[Main] %d WSDL files have been found in directory '%s'", (int)listWSDLFileNames.count(), szWSDLFilesDirectory);

	// Iterate over each WSDL files
	QStringList::const_iterator iter;
	int iSuccessCount = 0;
	for(iter = listWSDLFileNames.constBegin(); iter != listWSDLFileNames.constEnd(); ++iter)
	{
		QString szFilename = *iter;
		QString szFilePath = dirWDSLFiles.filePath(szFilename);
		bGoOn = processWSDLFile(szFilename, szFilePath, szServiceName, szOutputDirectory, szResourcePath, szNamespace, bFileGenerated, pListGeneratedFiles);
		if (bGoOn) {
			iSuccessCount++;
		}else{
			qDebug("[Main] Error to process: %s", qPrintable(szFilename));
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

	qDebug("[Main] %d/%d WSDL files have been processed successfully '%s'", iSuccessCount, (int)listWSDLFileNames.count(), szWSDLFilesDirectory);

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
