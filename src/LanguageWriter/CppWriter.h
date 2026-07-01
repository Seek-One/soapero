//
// Created by ebeuque on 01/07/2026.
//

#ifndef SOAPERO_LANGUAGEWRITER_CPPWRITER_H
#define SOAPERO_LANGUAGEWRITER_CPPWRITER_H

#include <QString>
#include <QTextStream>

#ifndef CRLF
#define CRLF "\r\n"
#endif

class CppWriter
{
public:
	CppWriter(QTextStream& os);
	virtual ~CppWriter();

public:
	// Includes
	void writeIncludeFileSystem(const QString& szIncludeFile) const;
	void writeIncludeFileLocal(const QString& szIncludeFile) const;

	void writeDeclarationVariable(const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);
	void writeDeclarationVariableList(const QString& szListType, const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);

private:
	QTextStream& m_os;
};


#endif //SOAPERO_LANGUAGEWRITER_CPPWRITER_H