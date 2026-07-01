//
// Created by ebeuque on 01/07/2026.
//

#ifndef SOAPERO_LANGUAGEWRITER_QTCPPWRITER_H
#define SOAPERO_LANGUAGEWRITER_QTCPPWRITER_H

#include <QString>

#include "CppWriter.h"

class QtCppWriter : public CppWriter
{
public:
	QtCppWriter(QTextStream& os);
	virtual ~QtCppWriter();

	void writeDeclarationVariableList(const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);
};

#endif //SOAPERO_QTCPPWRITER_H