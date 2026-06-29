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
	void doWriteCppIncludes(QTextStream& os, const TypeSharedPtr& pType) const override;
	void doWriteCppClass(QTextStream& os, const TypeSharedPtr& pType) const override;

	// Simple type
	void doWriteHeaderClass(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;
	void doWriteCppClass(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const;

	// Complex type
	void doWriteHeaderClass(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const;
	void doWriteCppClass(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace = QString()) const;

	void startCppClass(QTextStream& os, const QString& szClassName, const ComplexTypeSharedPtr& pComplexType) const;
	void endCppClass(QTextStream& os) const;
};


#endif //SOAPERO_TARGETENGINE_QTCPPTARGETENGINE_H