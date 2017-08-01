/*
 * TypeListBuilder.cpp
 *
 *  Created on: 17 juil. 2017
 *      Author: lgruber
 */

#include <QDateTime>
#include <QCoreApplication>
#include <QStringList>
#include <QDir>

#include "TypeListBuilder.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

TypeListBuilder::TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement)
{
	m_pListType = pListType;
	m_pListElement = pListElement;
	m_pService = pService;
	m_szPrefix = "";
}

TypeListBuilder::~TypeListBuilder()
{

}

void TypeListBuilder::setPrefix(const QString& szPrefix)
{
	m_szPrefix = szPrefix;
}

QString TypeListBuilder::getPrefix() const
{
	return m_szPrefix;
}

void TypeListBuilder::setNamespace(const QString& szNamespace)
{
	m_szNamespace = szNamespace;
}

QString TypeListBuilder::getNamespace() const
{
	return m_szNamespace;
}

void TypeListBuilder::setFilename(const QString& szFilename)
{
	m_szFilename = szFilename;
}

QString TypeListBuilder::getFilename() const
{
	return m_szFilename;
}

void TypeListBuilder::setDirname(const QString& szDirname)
{
	m_szDirname = szDirname;
}

QString TypeListBuilder::getDirname() const
{
	return m_szDirname;
}

void TypeListBuilder::buildHeaderFiles()
{
	TypeList::const_iterator type;
	RequestResponseElementList::const_iterator element;

	QDir dir(m_szDirname);
	if(!dir.mkpath("types")) {
		qDebug("[TypeListBuilder::buildHeaderFiles] Cannot create types dir");
	}
	if(!dir.mkpath("messages")) {
		qDebug("[TypeListBuilder::buildHeaderFiles] Cannot create messages dir");
	}

	for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {
		if(!(*type)->getLocalName().isEmpty()) {
			buildHeaderFile(*type);
		}
	}

	for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {
		if(!(*type)->getLocalName().isEmpty()) {
			buildHeaderFile(*type);
		}
	}

	for(element = m_pListElement->constBegin(); element != m_pListElement->constEnd(); ++element) {
		if(!(*element)->getLocalName().isEmpty()) {
			buildHeaderFile(*element);
		}
	}

	buildHeaderFile(m_pService);
}

void TypeListBuilder::buildCppFiles()
{
	TypeList::const_iterator type;
	RequestResponseElementList::const_iterator element;

	for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {
		if(!(*type)->getLocalName().isEmpty()) {
			buildCppFile(*type);
		}
	}

	for(element = m_pListElement->constBegin(); element != m_pListElement->constEnd(); ++element) {
		if(!(*element)->getLocalName().isEmpty()) {
			buildCppFile(*element);
		}
	}

	buildCppFile(m_pService);
}

void TypeListBuilder::buildHeaderFile(const TypeSharedPtr& pType)
{
	QString szHeaderFilename = pType->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "types" + QDir::separator() + szHeaderFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = (!m_szPrefix.isEmpty() ? m_szPrefix.toUpper() + "_" : "") + pType->getQualifiedName().toUpper() + "_H_";

		os << "/*" << CRLF;
		os << " * " << szHeaderFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#ifndef " << szDefine << CRLF;
		os << "#define " << szDefine << CRLF;
		os << CRLF;

		buildHeaderIncludeType(os, pType);

		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassType(os, pType);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szHeaderFilename = pElement->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "messages" + QDir::separator() + szHeaderFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = (!m_szPrefix.isEmpty() ? m_szPrefix.toUpper() + "_" : "") + pElement->getQualifiedName().toUpper() + "_H_";

		os << "/*" << CRLF;
		os << " * " << szHeaderFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#ifndef " << szDefine << CRLF;
		os << "#define " << szDefine << CRLF;
		os << CRLF;

		buildHeaderIncludeElement(os, pElement);

		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassElement(os, pElement);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const ServiceSharedPtr& pService)
{
	QString szHeaderFilename = pService->getName() + ".h";

	QFile file(m_szDirname + QDir::separator() + szHeaderFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = (!m_szPrefix.isEmpty() ? m_szPrefix.toUpper() + "_" : "") + pService->getName().toUpper() + "_H_";

		os << "/*" << CRLF;
		os << " * " << szHeaderFilename << ".h" << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#ifndef " << szDefine << CRLF;
		os << "#define " << szDefine << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassService(os, m_pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const TypeSharedPtr& pType)
{
	QString szCppFilename = pType->getQualifiedName() + ".cpp";
	QString szHeaderFilename = pType->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "types" + QDir::separator() + szCppFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		os << "/*" << CRLF;
		os << " * " << szCppFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassType(os, pType);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szCppFilename = pElement->getQualifiedName() + ".cpp";
	QString szHeaderFilename = pElement->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "messages" + QDir::separator() + szCppFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		os << "/*" << CRLF;
		os << " * " << szCppFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassElement(os, pElement);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const ServiceSharedPtr& pService)
{
	QString szCppFilename = pService->getName() + ".cpp";
	QString szHeaderFilename = pService->getName() + ".h";

	QFile file(m_szDirname + QDir::separator() + szCppFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		os << "/*" << CRLF;
		os << " * " << szCppFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassService(os, pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName();

	os << "namespace " << pType->getNamespace().toUpper() << " {" << CRLF;
	os << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		os << "class " << szClassname << CRLF;
		os << "{" << CRLF;
		os << "public:" << CRLF;

		if(pSimpleType->isRestricted()) {
			if(pSimpleType->getVariableType() == SimpleType::String &&
					pSimpleType->getEnumerationValues().count() > 0) {
				os << "\t" << pSimpleType->getEnumerationDeclaration() << CRLF;
				os << CRLF;
			}
		}

		os << "\t" << szClassname << "();" << CRLF;
		os << "\tvirtual ~" << szClassname << "();" << CRLF;
		os << CRLF;

		buildHeaderClassSimpleType(os, pSimpleType);

		os << "};" << CRLF;
		os << CRLF;

	} else if(pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		if(pComplexType->getExtensionType().isNull()) {
			os << "class " << szClassname << CRLF;
		}else{
			QString szExtensionName = pComplexType->getExtensionType()->getQualifiedName();

			QString szExtendedClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;
			os << "class " << szClassname << " : public " << szExtendedClassname << CRLF;
		}
		os << "{" << CRLF;
		os << "public:" << CRLF;
		os << "\t" << szClassname << "();" << CRLF;
		os << "\tvirtual ~" << szClassname << "();" << CRLF;
		os << CRLF;

		buildHeaderClassComplexType(os, pComplexType);

		os << "};" << CRLF;
		os << CRLF;
	}

	os << "}" << CRLF;
}

void TypeListBuilder::buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
	os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
	os << "\t" << pSimpleType->getSerializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getEnumConvertDeclaration() << CRLF;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\t" << pSimpleType->getVariableDeclaration() << CRLF;
}

void TypeListBuilder::buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		os << "public:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if(!(*attr)->getType()) {
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable((*attr)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());

				os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
				os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
				os << CRLF;

			}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());

				os << "\t" << pComplexType->getSetterDeclaration() << CRLF ;
				os << "\t" << pComplexType->getGetterDeclaration() << CRLF ;
				os << CRLF;
			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {

			ComplexTypeSharedPtr pType = qSharedPointerCast<ComplexType>((*element)->getType());

			if(!(*element)->getType()) {
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable((*element)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*element)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());

				os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
				os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
				os << CRLF;

			}else if( (*element)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());

				os << "\t" << pComplexType->getSetterDeclaration() << CRLF ;
				os << "\t" << pComplexType->getGetterDeclaration() << CRLF ;
				os << CRLF;
			}
		}
		os << "\t" << pComplexType->getSerializerDeclaration() << CRLF;

		os << "private:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if(!(*attr)->getType()) {
				continue;
			}
			if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());
				os << "\t" << pSimpleType->getVariableDeclaration() << CRLF ;

			}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());
				os << "\t" << pComplexType->getVariableDeclaration() << CRLF ;
			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {

			if(!(*element)->getType()) {
				continue;
			}

			if( (*element)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());
				os << "\t" << pSimpleType->getVariableDeclaration() << CRLF ;

			}else if( (*element)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());
				os << "\t" << pComplexType->getVariableDeclaration() << CRLF;

			}
		}
	}
}

void TypeListBuilder::buildHeaderClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getQualifiedName();
	ComplexTypeSharedPtr pComplexType = pElement->getComplexType();

	os << "namespace " << pElement->getNamespace().toUpper() << " {" << CRLF;
	os << CRLF;

	if(!pComplexType.isNull()) {
		os << "class " << szClassname << CRLF;
		os << "{" << CRLF;
		os << "public:" << CRLF;
		os << "\t" << szClassname << "();" << CRLF;
		os << "\tvirtual ~" << szClassname << "();" << CRLF;
		os << CRLF;

		buildHeaderClassComplexType(os, pComplexType);

		os << "};" << CRLF;
		os << CRLF;
	}

	os << "}" << CRLF;
}

void TypeListBuilder::buildHeaderClassService(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QString szClassname = pService->getName();

	os << "class " << szClassname << CRLF;
	os << "{" << CRLF;
	os << "public:" << CRLF;
	os << "\t" << szClassname << "();" << CRLF;
	os << "\tvirtual ~" << szClassname << "();" << CRLF;
	os << CRLF;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		os << "\t" << (*operation)->getOperationDeclaration() << CRLF;
	}

	os << "};" << CRLF;
}

void TypeListBuilder::buildHeaderIncludeType(QTextStream& os, const TypeSharedPtr& pType) const
{

	os << "#include <QString>" << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		if(!pSimpleType->getVariableTypeFilenameString().isEmpty()) {
			os << "#include \"" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
		}
	}else if(pType->getClassType() == Type::TypeComplex){

		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
		AttributeList::const_iterator attr;

		ElementListSharedPtr pListElements = pComplexType->getElementList();
		ElementList::const_iterator element;

		if(pListAttributes->count() > 0 || pListElements->count() > 0) {

			QStringList list;

			for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
				if(!(*attr)->getType()) {
					continue;
				}
				if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
					SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());

					if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
						os << "#include \"" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
						list.append(pSimpleType->getVariableTypeFilenameString());

					}
				}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
					ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());

					if(!list.contains(pComplexType->getQualifiedName())) {
						os << "#include \"" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
						list.append(pComplexType->getQualifiedName());

					}

				}
			}
			for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {

				if(!(*element)->getType()) {
					continue;
				}

				if( (*element)->getType()->getClassType() == Type::TypeSimple) {
					SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());
					if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
						os << "#include \"" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
						list.append(pSimpleType->getVariableTypeFilenameString());

					}

				}else if( (*element)->getType()->getClassType() == Type::TypeComplex) {
					ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());
					if(!list.contains(pComplexType->getQualifiedName())) {
						os << "#include \"" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
						list.append(pComplexType->getQualifiedName());

					}

				}
			}
		}
	}
}

void TypeListBuilder::buildHeaderIncludeElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	ComplexTypeSharedPtr pComplexType = pElement->getComplexType();

	buildHeaderIncludeType(os, pComplexType);
}

void TypeListBuilder::buildHeaderIncludeService(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QStringList list;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		if(!list.contains((*operation)->getInputMessage()->getQualifiedName())) {
			os << "#include \"" << (*operation)->getInputMessage()->getQualifiedName() << ".h\"" << CRLF;
			list.append((*operation)->getInputMessage()->getQualifiedName());
		}
	}

	os << "};" << CRLF;
}

void TypeListBuilder::buildCppClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getQualifiedName();

	os << "namespace " << pType->getNamespace().toUpper() << " {" << CRLF;
	os << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		if(pSimpleType->isRestricted() && pSimpleType->getVariableType() == SimpleType::String &&
				pSimpleType->getEnumerationValues().count() > 0) {

			os << szClassname << "::" << szClassname << "()" << CRLF;
			os << "{" << CRLF;
			os << "\t" << pSimpleType->getVariableName() << " = " << pSimpleType->getEnumerationValues()[0] << ";" << CRLF;
			os << "}" << CRLF;
			os << CRLF;
		}else{
			os << szClassname << "::" << szClassname << "() {}" << CRLF;
		}
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		buildCppClassSimpleType(os, pSimpleType);

		os << CRLF;

	} else if(pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		if(pComplexType->getExtensionType().isNull()) {
			os << szClassname << "::" << szClassname << "() {}" << CRLF;
			os << szClassname << "::~" << szClassname << "() {}" << CRLF;
			os << CRLF;
		}else{
			QString szExtensionName = pComplexType->getExtensionType()->getQualifiedName();
			QString szExtendedClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;

			os << szClassname << "::" << szClassname << "() :" << szExtendedClassname << "() {}" << CRLF;
			os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		}
		os << CRLF;

		buildCppClassComplexType(os, pComplexType);

		os << CRLF;
	}

	os << "}" << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pSimpleType->getQualifiedName();

	os << pSimpleType->getSetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getGetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getSerializerDefinition(szClassname) << CRLF;
	os << pSimpleType->getEnumConvertDefinition(szClassname) << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pComplexType->getQualifiedName();

	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if(!(*attr)->getType()) {
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable((*attr)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*attr)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());

				os << pSimpleType->getSetterDefinition(szClassname) << CRLF ;
				os << pSimpleType->getGetterDefinition(szClassname) << CRLF ;
				os << CRLF;

			}else if( (*attr)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());

				os << pComplexType->getSetterDefinition(szClassname) << CRLF ;
				os << pComplexType->getGetterDefinition(szClassname) << CRLF ;
				os << CRLF;
			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {

			ComplexTypeSharedPtr pType = qSharedPointerCast<ComplexType>((*element)->getType());

			if(!(*element)->getType()) {
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable((*element)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*element)->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());

				os << pSimpleType->getSetterDefinition(szClassname) << CRLF ;
				os << pSimpleType->getGetterDefinition(szClassname) << CRLF ;
				os << CRLF;

			}else if( (*element)->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());

				os << pComplexType->getSetterDefinition(szClassname) << CRLF ;
				os << pComplexType->getGetterDefinition(szClassname) << CRLF ;
				os << CRLF;
			}
		}

		os << pComplexType->getSerializerDefinition(szClassname) << CRLF;
	}
}

void TypeListBuilder::buildCppClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getQualifiedName();
	ComplexTypeSharedPtr pComplexType = pElement->getComplexType();
	pComplexType->setLocalName(pElement->getLocalName());
	pComplexType->setNamespace(pElement->getNamespace());

	os << "namespace " << pElement->getNamespace().toUpper() << " {" << CRLF;
	os << CRLF;

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		buildCppClassComplexType(os, pComplexType);

		os << CRLF;
	}

	os << "}" << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppClassService(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QString szClassname = pService->getName();

	os << szClassname << "::" << szClassname << "() {}" << CRLF;
	os << szClassname << "::~" << szClassname << "() {}" << CRLF;
	os << CRLF;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		os << (*operation)->getOperationDefinition(szClassname) << CRLF;
	}
}
