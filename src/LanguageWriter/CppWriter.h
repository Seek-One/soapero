//
// Created by ebeuque on 01/07/2026.
//

#ifndef SOAPERO_LANGUAGEWRITER_CPPWRITER_H
#define SOAPERO_LANGUAGEWRITER_CPPWRITER_H

#include <QString>
#include <QTextStream>

#include "../common.h"

class CppWriter
{
public:
	CppWriter(QTextStream& os);
	virtual ~CppWriter();

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

	// Variable declaration
	void writeDeclarationVariable(const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);
	void writeDeclarationVariableList(const QString& szListType, const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);

private:
	QTextStream& m_os;
};


#endif //SOAPERO_LANGUAGEWRITER_CPPWRITER_H