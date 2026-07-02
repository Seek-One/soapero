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

void CppWriter::setClassName(const QString& szClassName)
{
	m_szClassName = szClassName;
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

void CppWriter::writeClassMethodDeclaration(const QString& szReturnType, const QString& szMethodName, const QString& szParamType, const QString& szParamName)
{
	m_os << "\t" << szReturnType << " " << szMethodName << "(" << szParamType << " " << szParamName << ");" << CRLF;
}

void CppWriter::writeClassMethodDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName)
{
	m_os << szReturnType << " " << szClassName << "::" << szMethodName << "(";
	if (!szParamType.isEmpty()) {
		m_os << szParamType << " " << szParamName;
	}
	m_os << ")" << CRLF;
	m_os << "{" << CRLF;
}

void CppWriter::writeClassMethodDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType)
{
	writeClassMethodDefinitionStart(szClassName, szMethodName, szReturnType, QString(), QString());
}

void CppWriter::writeClassMethodDefinitionStart(const QString& szMethodName, const QString& szReturnType)
{
	writeClassMethodDefinitionStart(m_szClassName, szMethodName, szReturnType);
}

void CppWriter::writeClassMethodDefinitionStart(const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName)
{
	writeClassMethodDefinitionStart(m_szClassName, szMethodName, szReturnType, szParamType, szParamName);
}

void CppWriter::writeClassMethodDefinitionEnd()
{
	m_os << "}" << CRLF << CRLF;
}

void CppWriter::writeClassMethodConstDeclaration(const QString& szReturnType, const QString& szMethodName, const QString& szParamType, const QString& szParamName)
{
	m_os << "\t" << szReturnType << " " << szMethodName << "(" << szParamType << " " << szParamName << ") const;" << CRLF;
}

void CppWriter::writeClassMethodConstDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName)
{
	m_os << szReturnType << " " << szClassName << "::" << szMethodName << "(";
	if (!szParamType.isEmpty()) {
		m_os << szParamType << " " << szParamName;
	}
	m_os << ") const" << CRLF;
	m_os << "{" << CRLF;
}

void CppWriter::writeClassMethodConstDefinitionStart(const QString& szClassName, const QString& szMethodName, const QString& szReturnType)
{
	writeClassMethodConstDefinitionStart(szClassName, szMethodName, szReturnType, QString(), QString());
}

void CppWriter::writeClassMethodConstDefinitionStart(const QString& szMethodName, const QString& szReturnType)
{
	writeClassMethodConstDefinitionStart(m_szClassName, szMethodName, szReturnType);
}

void CppWriter::writeClassMethodConstDefinitionStart(const QString& szMethodName, const QString& szReturnType, const QString& szParamType, const QString& szParamName)
{
	writeClassMethodConstDefinitionStart(m_szClassName, szMethodName, szReturnType, szParamType, szParamName);
}

void CppWriter::writeClassMethodConstDefinitionEnd()
{
	m_os << "}" << CRLF << CRLF;
}