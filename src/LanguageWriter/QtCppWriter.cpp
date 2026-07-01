//
// Created by ebeuque on 01/07/2026.
//

#include "QtCppWriter.h"

QtCppWriter::QtCppWriter(QTextStream& os) : CppWriter(os)
{

}

QtCppWriter::~QtCppWriter()
{

}

void QtCppWriter::writeDeclarationVariableList(const QString& szTypeName, const QString& szVariableName, bool bTypePointer)
{
	CppWriter::writeDeclarationVariableList("QList", szTypeName, szVariableName, bTypePointer);
}