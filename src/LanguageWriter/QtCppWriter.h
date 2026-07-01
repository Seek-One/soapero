//
// Created by ebeuque on 01/07/2026.
//

#ifndef SOAPERO_LANGUAGEWRITER_QTCPPWRITER_H
#define SOAPERO_LANGUAGEWRITER_QTCPPWRITER_H

#include <QString>

#include "CppWriter.h"

/*
 * Qt C++ writer extention to the C++ writer.
 * This class must extend the base class by adding Qt specific usage in code writing.
 */
class QtCppWriter : public CppWriter
{
public:
	QtCppWriter(QTextStream& os);
	virtual ~QtCppWriter();

	void writeDeclarationVariableList(const QString& szTypeName, const QString& szVariableName, bool bTypePointer = false);
};

#endif //SOAPERO_QTCPPWRITER_H