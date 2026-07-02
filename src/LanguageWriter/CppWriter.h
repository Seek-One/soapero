//
// Created by ebeuque on 01/07/2026.
//

#ifndef SOAPERO_LANGUAGEWRITER_CPPWRITER_H
#define SOAPERO_LANGUAGEWRITER_CPPWRITER_H

#include <QString>
#include <QTextStream>

#include "../common.h"

/*
 * Base class for C++ writer.
 * This class must contain only basic function to write C++ code without using any framework.
 */
class CppWriter
{
public:
	CppWriter(QTextStream& os);
	virtual ~CppWriter();

public:
	void setClassName(const QString& szClassName);

public:
	// Header guardian
	void writeDeclarationGuardStart(const QString& szHeaderGuard);
	void writeDeclarationGuardEnd(const QString& szHeaderGuard);

	// Includes
	void writeIncludeFileSystem(const QString& szIncludeFile);
	void writeIncludeFileLocal(const QString& szIncludeFile);

	// Namespace
	void writeNamespaceStart(const QString& szNamespace);
	void writeNamespaceEnd(const QString& szNamespace);

	// Class variable declaration
	void writeDeclarationVariable(const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);
	void writeDeclarationVariableList(const QString& szListType, const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);

	// Class methods non-const
	void writeClassMethodDeclaration(const QString& szReturnType, const QString& szMethodName, const QString& szParamType, const QString& szParamName);
	void writeClassMethodDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName);
	void writeClassMethodDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType);
	void writeClassMethodDefinitionStart(const QString& szMethodName, const QString& szReturnType);
	void writeClassMethodDefinitionStart(const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName);
	void writeClassMethodDefinitionEnd();
	// Class methods const
	void writeClassMethodConstDeclaration(const QString& szReturnType, const QString& szMethodName, const QString& szParamType, const QString& szParamName);
	void writeClassMethodConstDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName);
	void writeClassMethodConstDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType);
	void writeClassMethodConstDefinitionStart(const QString& szMethodName, const QString& szReturnType);
	void writeClassMethodConstDefinitionStart(const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName);
	void writeClassMethodConstDefinitionEnd();

private:
	QTextStream& m_os;

	QString m_szClassName;
};


#endif //SOAPERO_LANGUAGEWRITER_CPPWRITER_H