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
	void doWriteDeclarationSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName, SetterParamMode iParamMode) const override;
	void doWriteDeclarationGetterList(QTextStream& os, const QString& szFuncName, const QString& szMemberType, GetterReturnMode iReturnMode) const override;
	void doWriteDefinitionSetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const override;
	void doWriteDefinitionAddList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szParamType, const QString& szParamName, const QString& szMemberName, SetterParamMode iParamMode) const override;
	void doWriteDefinitionGetterList(QTextStream& os, const QString& szClassName, const QString& szFuncName, const QString& szMemberType, const QString& szMemberName, GetterReturnMode iReturnMode) const override;

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
	void doWriteDefinitionGetterSetter(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const;
	void doWriteDefinitionSerializerDeserializer(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const;
	void doWriteDeclarationIsNull(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteDefinitionIsNull(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType, const QString& szClassName) const;

	// Complex type
	void doWriteDeclarationClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteDefinitionClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace = QString()) const;
	void doWriteDefinitionSerializer(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName, const QString& szTargetNamespace = QString()) const;
	void doWriteDefinitionDeserializer(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName, const QString& szTargetNamespace = QString()) const;
	void doWriteDeclarationIsNull(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteDefinitionIsNull(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName) const;
	void doWriteDeclarationGetNamespaceDeclaration(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteDefinitionGetNamespaceDeclaration(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szClassName) const;

	// Element
	void doWriteDeclarationGetterSetter(QTextStream& os, const ElementSharedPtr& pElement) const;
	void doWriteDefinitionGetterSetter(QTextStream& os, const ElementSharedPtr& pElement, const QString& szClassName) const;
	void doWriteVariableDeclaration(QTextStream& os, const ElementSharedPtr& pElement) const;

	// Attribute
	void doWriteDeclarationGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute) const;
	void doWriteDefinitionGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute, const QString& szClassName) const;
	void doWriteVariableDeclaration(QTextStream& os, const AttributeSharedPtr& pAttribute) const;

	void startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const;
	void endCppClass(QTextStream& os, const QString& szClassName) const;

	static QString getBaseClassName(const QString& szPrefix, const ComplexTypeSharedPtr& pComplexType);
};


#endif //SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H