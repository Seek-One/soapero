//
// Created by ebeuque on 29/06/2026.
//

#ifndef SOAPERO_TARGETENGINE_TARGETENGINE_H
#define SOAPERO_TARGETENGINE_TARGETENGINE_H

#include <QDir>

#include "Model/Service.h"
#include "Model/Type.h"
#include "Model/RequestResponseElement.h"

#include "Utils/UniqueStringList.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

#define USE_COMPAT_TEST

/*
 * Base class for target engines
 */
class TargetEngine {
public:
	TargetEngine();
	virtual ~TargetEngine() = 0;

public:
	enum FileCategory {
		FileCategory_Unknown,
		FileCategory_Type,
		FileCategory_Message,
		FileCategory_Service,
	};

	void setOutputDirectory(const QDir& dir);
	void setPrefix(const QString& szPrefix);
	void setNamespace(const QString& szNamespace);

	void setService(const ServiceSharedPtr& pService);
	void setRequestResponseElementList(const RequestResponseElementListSharedPtr& pElementList);
	void setTypeList(const TypeListSharedPtr& pTypeList);

	void setGeneratedFileList(const QSharedPointer<UniqueStringList>& pGeneratedFilesList);

	bool build();

protected:
	virtual bool doBuild();
	virtual bool doBuildService(const ServiceSharedPtr& pService);
	virtual bool doBuildRequestResponseElementList(const RequestResponseElementListSharedPtr& pElementList);
	virtual bool doBuildRequestResponseElement(const RequestResponseElementSharedPtr& pElement);
	virtual bool doBuildTypeList(const TypeListSharedPtr& pTypeList);
	virtual bool doBuildType(const TypeSharedPtr& pType);

	TypeSharedPtr findTypeByName(const QString& szLocalName, const QString& szNamespace, const TypeListSharedPtr& pListIgnoredTypes = TypeListSharedPtr()) const;

	bool openFile(QFile& file, bool bCleanFirst = true) const;
	void closeFile(QFile& file) const;

protected:
	QDir m_outputDir;

	QString m_szNamespace;
	QString m_szPrefix;

	ServiceSharedPtr m_pService;
	RequestResponseElementListSharedPtr m_pElementList;
	TypeListSharedPtr m_pTypeList;

	QSharedPointer<UniqueStringList> m_pGeneratedFilesList;

};


#endif //SOAPERO_TARGETENGINE_TARGETENGINE_H