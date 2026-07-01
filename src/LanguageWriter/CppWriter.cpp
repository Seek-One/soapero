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

void CppWriter::writeDeclarationGuardStart(const QString& szHeaderGuard)
{
	m_os << "#ifndef " << szHeaderGuard << CRLF;
	m_os << "#define " << szHeaderGuard << CRLF;
	m_os << CRLF;
}

void CppWriter::writeDeclarationGuardEnd(const QString& szHeaderGuard)
{
	m_os << CRLF;
	m_os << "#endif // " << szHeaderGuard << CRLF;
}

void CppWriter::writeIncludeFileSystem(const QString& szIncludeFile)
{
	m_os << "#include <" << szIncludeFile << ">" << CRLF;
}

void CppWriter::writeIncludeFileLocal(const QString& szIncludeFile)
{
	m_os << "#include \"" << szIncludeFile << "\"" << CRLF;
}

void CppWriter::writeNamespaceStart(const QString& szNamespace)
{
	if(!szNamespace.isEmpty()) {
		m_os << "namespace " << szNamespace << " {" << CRLF;
	}
}

void CppWriter::writeNamespaceEnd(const QString& szNamespace)
{
	if(!szNamespace.isEmpty()) {
		m_os << "} // " << szNamespace << CRLF;
	}
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