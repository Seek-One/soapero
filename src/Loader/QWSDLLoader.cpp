//
// Created by ebeuque on 08/07/2026.
//

#include <QFile>
#include <QBuffer>
#include <QDir>

#include "Parser/QWSDLParser.h"
#include "Builder/FileBuilder.h"
#include "Builder/FileHelper.h"
#include "Builder/TypeListBuilder.h"

#include "QWSDLLoader.h"

QWSDLLoader::QWSDLLoader() {

}

QWSDLLoader::~QWSDLLoader()
{

}

void QWSDLLoader::setFileName(const QString& szFileName)
{
	m_szFileName = szFileName;
}

void QWSDLLoader::setFilePath(const QString& szFilePath)
{
	m_szFilePath = szFilePath;
}

void QWSDLLoader::setOriginalURL(const QString& szOriginalURL)
{
	m_szOriginalURL = szOriginalURL;
}

void QWSDLLoader::setServiceName(const QString& szServiceName)
{
	m_szServiceName = szServiceName;
}

void QWSDLLoader::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

void QWSDLLoader::setResourcePath(const QString& szResourcePath)
{
	m_szResourcePath = szResourcePath;
}

void QWSDLLoader::setOutputDirectory(const QString& szOutputDirectory)
{
	m_szOutputDirectory = szOutputDirectory;
}

bool QWSDLLoader::processWSDLFile(bool& bFileGenerated, QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	bool bRes;

	// Open current WSDL file
	qDebug("[Main] Processing file '%s'", qPrintable(m_szFileName));

	QFile file(m_szFilePath);
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
		if (!m_szOriginalURL.isEmpty()) {
			parser.setSchemaURI(m_szOriginalURL);
		}else {
			parser.setSchemaURI(m_szFileName);
		}
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
			if(m_szServiceName.isNull()){
				szCurrentServiceName = pService->getName();
			}else{
				szCurrentServiceName = m_szServiceName;
				pService->setName(m_szServiceName);
			}
			if(szCurrentServiceName.isNull()){
				qDebug("[Main] Service name is not defined");
				bRes = false;
			}
		}

		// Create directory output if not existing
		if(bRes){
			if(!QDir(m_szOutputDirectory).exists()) {
				qDebug("[Main] Creating non-existing directory '%s'", qPrintable(m_szOutputDirectory));
				bRes = QDir().mkdir(m_szOutputDirectory);
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
			builder.setNamespace(m_szNamespace);
			builder.setFilename("actionservice");
			builder.setDirname(m_szOutputDirectory);
			builder.build();

			bFileGenerated = true;
		}

		// Add class for base type
		if(bRes){
			QString szResourcesBasePath = QDir(m_szResourcePath).filePath("Base/xs/types");
			QDir dir(szResourcesBasePath);
			if(dir.exists()){
				foreach (QString f, dir.entryList(QDir::Files))
				{
					QString szSrcPath = QDir(szResourcesBasePath).filePath(f);
					QString szDstFullPath = FileHelper::buildPath(m_szOutputDirectory, "xs", "types", f);
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
			QString szResourcesServicePath = QDir(m_szResourcePath).filePath("Service");
			QDir dir = QDir(szResourcesServicePath);
			if(dir.exists()){
				// Copy services files
				foreach (QString f, dir.entryList(QDir::Files))
				{
					QString szSrcPath = QDir(szResourcesServicePath).filePath(f);
					QString szDstPath = QDir(m_szOutputDirectory).filePath(f);

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
							text.replace(QString("namespace SOAPERO"), QString("namespace %0").arg(m_szNamespace));
							text.replace(QString("SOAPERO_SERVICE_H_"), QString("%0_SERVICE_H_").arg(m_szNamespace));

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