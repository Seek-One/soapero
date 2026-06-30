//
// Created by ebeuque on 29/06/2026.
//

#ifndef SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H
#define SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H

#include "TargetEngine/CppTargetEngine.h"

class QtCppTargetEngine : public CppTargetEngine
{
public:
	QtCppTargetEngine();
	virtual ~QtCppTargetEngine();

private:
	// CPP files creation
	void doWriteNamespaceTargetInfos(QTextStream& os, const QString& szPrefix, const QString& szNamespace, const QString& szNamespaceURI) const override;

	// CPP class creation
	void doWriteDeclarationSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName) const override;

	// Service
	void doWriteDeclarationIncludes(QTextStream& os, const ServiceSharedPtr& pService) const override;
	void doWriteDeclarationClass(QTextStream& os, const ServiceSharedPtr& pService) const override;
	void doWriteDefinitionClass(QTextStream& os, const ServiceSharedPtr& pService) const override;

	// Request response elements files
	void doWriteDeclarationIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;
	void doWriteDeclarationClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;
	void doWriteDefinitionClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;

	// Types files
	void doWriteDeclarationIncludes(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteDeclarationClass(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteDeclarationSerializer(QTextStream& os, const TypeSharedPtr& pType) const;
	void doWriteDeclarationDeserializer(QTextStream& os, const TypeSharedPtr& pType) const;
	void doWriteDefinitionIncludes(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteDefinitionClass(QTextStream& os, const TypeSharedPtr& pType) const override;

	// Simple type
	void doWriteDeclarationClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteDeclarationGetterSetter(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteDefinitionClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;

	// Complex type
	void doWriteDeclarationClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteDefinitionClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace = QString()) const;

	// Attribute
	void doWriteDeclarationGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute) const;


	void startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const;
	void endCppClass(QTextStream& os, const QString& szClassName) const;

	static QString getBaseClassName(const QString& szPrefix, const ComplexTypeSharedPtr& pComplexType);
};


#endif //SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H