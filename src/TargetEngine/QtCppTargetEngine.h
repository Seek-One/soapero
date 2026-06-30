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
	void doWriteHeaderSetterList(QTextStream& os, const QString& szFuncName, const QString& szParamType, const QString& szParamName) const override;

	// Service
	void doWriteHeaderIncludes(QTextStream& os, const ServiceSharedPtr& pService) const override;
	void doWriteHeaderClass(QTextStream& os, const ServiceSharedPtr& pService) const override;
	void doWriteCppClass(QTextStream& os, const ServiceSharedPtr& pService) const override;

	// Request response elements files
	void doWriteHeaderIncludes(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;
	void doWriteHeaderClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;
	void doWriteCppClass(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const override;

	// Types files
	void doWriteHeaderIncludes(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteHeaderSerializer(QTextStream& os, const TypeSharedPtr& pType) const;
	void doWriteHeaderDeserializer(QTextStream& os, const TypeSharedPtr& pType) const;
	void doWriteCppIncludes(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteCppClass(QTextStream& os, const TypeSharedPtr& pType) const override;

	// Simple type
	void doWriteHeaderClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteHeaderGetterSetter(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteCppClassContent(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;

	// Complex type
	void doWriteHeaderClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteCppClassContent(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace = QString()) const;

	// Attribute
	void doWriteHeaderGetterSetter(QTextStream& os, const AttributeSharedPtr& pAttribute) const;


	void startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const;
	void endCppClass(QTextStream& os, const QString& szClassName) const;

	static QString getBaseClassName(const QString& szPrefix, const ComplexTypeSharedPtr& pComplexType);
};


#endif //SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H