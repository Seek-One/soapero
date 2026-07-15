//
// Created by ebeuque on 29/06/2026.
//

#ifndef SOAPERO_TARGETENGINE_CPPTARGETENGINE_H
#define SOAPERO_TARGETENGINE_CPPTARGETENGINE_H

#include "Model/SimpleType.h"

#include "TargetEngine.h"

class CppTargetEngine : public TargetEngine
{
public:
	CppTargetEngine();
	virtual ~CppTargetEngine();

protected:
	enum GetterReturnMode {
		GetterReturnModeDefault = 0,
		GetterReturnModeConst,
		GetterReturnModePointer,
		GetterReturnModeVariant,
	};

	enum SetterParamMode {
		SetterParamModeDefault = 0,
		SetterParamModeConst,
		SetterParamModePointer,

		SetterParamModeConst2, // Compat
	};

protected:
	// CPP files creation
	static QString getHeaderPath(const QString& szNamespace, const QString& szCategory, const QString& szFilename, FileCategory iOrigin);
	virtual void doWriteFileDescription(QTextStream& os, const QString& szFilename) const;
	virtual void doWriteClassInfos(QTextStream& os, const QString& szPrefix, const QString& szSchemaURI, const QString& szNamespace, const QString& szNamespaceURI) const;

	// CPP class creation
	void doWriteDeclarationClassStart(QTextStream& os, const QString& szClassName) const;
	virtual void doWriteDeclarationClassStart(QTextStream& os, const QString& szClassName, const QString& szBaseClass) const;
	virtual void doWriteDeclarationClassEnd(QTextStream& os, const QString& szClassName) const;
	virtual void doWriteDeclarationClassInitializers(QTextStream& os, const QString& szClassName, bool bEnumeration) const;
	virtual void doWriteDeclarationGetter(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const;
	virtual void doWriteDeclarationGetterList(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const;
	virtual void doWriteDeclarationSetter(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const;
	virtual void doWriteDeclarationSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const;
	virtual void doWriteDeclarationAddList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const;
	virtual void doWriteDefinitionGetter(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const;
	virtual void doWriteDefinitionGetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const;
	virtual void doWriteDefinitionSetter(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const;
	virtual void doWriteDefinitionSetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const;
	virtual void doWriteDefinitionAddList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const;
	virtual void doWriteDeclarationVariantIs(QTextStream& os, const QString& szFuncName) const;
	virtual void doWriteDefinitionVariantIs(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName) const;

	// Service files
	static QString getHeaderGuard(const QString& szPrefix, const ServiceSharedPtr& pService);
	bool doBuildService(const ServiceSharedPtr& pService) override;
	virtual bool doBuildHeaderFile(const ServiceSharedPtr& pService);
	virtual bool doBuildCppFile(const ServiceSharedPtr& pService);
	virtual void doWriteDeclarationIncludes(QTextStream& os, const ServiceSharedPtr& pService) const;
	virtual void doWriteDeclarationClass(QTextStream& os, const ServiceSharedPtr& pService) const;
	virtual void doWriteDefinitionIncludes(QTextStream& os, const TypeSharedPtr& pType) const;
	virtual void doWriteDefinitionClass(QTextStream& os, const ServiceSharedPtr& pService) const;

	// Request response elements files
	static QString getHeaderFileName(const RequestResponseElementSharedPtr& pElement);
	static QString getHeaderFilePath(const RequestResponseElementSharedPtr& pRequestResponseElement, FileCategory iOrigin);
	static QString getCppFileName(const RequestResponseElementSharedPtr& pElement);
	static QString getHeaderGuard(const QString& szPrefix, const RequestResponseElementSharedPtr& pElement);
	bool doBuildRequestResponseElement(const RequestResponseElementSharedPtr& pElement) override;
	virtual bool doBuildHeaderFile(const RequestResponseElementSharedPtr& pElement);
	virtual bool doBuildCppFile(const RequestResponseElementSharedPtr& pElement);
	virtual void doWriteDeclarationIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;
	virtual void doWriteDeclarationClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;
	virtual void doWriteDefinitionClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const;

	// Types files
	static QString getHeaderFileName(const TypeSharedPtr& pType);
	static QString getHeaderFilePath(const TypeSharedPtr& pType, FileCategory iOrigin);
	static QString getCppFileName(const TypeSharedPtr& pType);
	static QString getHeaderGuard(const QString& szPrefix, const TypeSharedPtr& pType);
	bool doBuildType(const TypeSharedPtr& pType) override;
	virtual bool doBuildHeaderFile(const TypeSharedPtr& pType);
	virtual bool doBuildCppFile(const TypeSharedPtr& pType);
	virtual void doWriteDeclarationIncludes(QTextStream& os, const TypeSharedPtr& pType) const;
	virtual void doWriteDeclarationClass(QTextStream& os, const TypeSharedPtr& pType) const;
	virtual void doWriteDefinitionClass(QTextStream& os, const TypeSharedPtr& pType) const;

	// Simple types files
	static QString getHeaderFileName(const SimpleTypeSharedPtr& pSimpleType);
	static QString getHeaderFilePath(const SimpleTypeSharedPtr& pSimpleType, FileCategory iOrigin);
	static QString getSimpleTypeNamespace(const SimpleTypeSharedPtr& pSimpleType);

	// Complex types files
	static QString getComplexTypeHeaderPath(const ComplexTypeSharedPtr& pComplexType, FileCategory iOrigin);
};
#endif //SOAPERO_TARGETENGINE_CPPTARGETENGINE_H