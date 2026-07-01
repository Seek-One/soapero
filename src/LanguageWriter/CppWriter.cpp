//
// Created by ebeuque on 01/07/2026.
//

#include "CppWriter.h"

CppWriter::CppWriter(QTextStream& os) : m_os(os)
{

}

CppWriter::~CppWriter()
{

}

void CppWriter::writeDeclarationVariable(const QString& szTypeName, const QString& szVariableName, bool bTypePointer)
{
	m_os << "\t" << szTypeName;
	if (bTypePointer) {
		m_os << "*";
	}
	m_os << " " << szVariableName << ";" << CRLF;
}

void CppWriter::writeDeclarationVariableList(const QString& szListType, const QString& szTypeName, const QString& szVariableName, bool bTypePointer)
{
	m_os << "\t" << szListType << "<" << szTypeName;
	if (bTypePointer) {
		m_os << "*";
	}
	m_os << "> " << szVariableName << ";" << CRLF;
}