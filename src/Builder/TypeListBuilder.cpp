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

#include "../Utils/StringUtils.h"
#include "TypeListBuilder.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

TypeListBuilder::TypeListBuilder(const ServiceSharedPtr& pService, const TypeListSharedPtr& pListType, const RequestResponseElementListSharedPtr& pListElement, const QSharedPointer<UniqueStringList>& pListGeneratedFiles)
{
	m_pListType = pListType;
	m_pListElement = pListElement;
	m_pService = pService;

	m_pListGeneratedFiles = pListGeneratedFiles;

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
		if(!(*type)->getLocalName().isEmpty() && ((*type)->getClassType() != Type::TypeUnknown)) {
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
		if(!(*type)->getLocalName().isEmpty() == ((*type)->getClassType() != Type::TypeUnknown)) {
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
	if(file.exists()) {
		file.remove();
	}
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

		m_pListGeneratedFiles->append(QString("types") + QDir::separator() + szHeaderFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szHeaderFilename = pElement->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "messages" + QDir::separator() + szHeaderFilename);
	if(file.exists()) {
		file.remove();
	}
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

		m_pListGeneratedFiles->append(QString("messages") + QDir::separator() + szHeaderFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderFile(const ServiceSharedPtr& pService)
{
	QString szHeaderFilename = pService->getName() + ".h";

	QFile file(m_szDirname + QDir::separator() + szHeaderFilename);
	if(file.exists()) {
		file.remove();
	}
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

		buildHeaderIncludeService(os, m_pService);

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildHeaderClassService(os, m_pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		os << "#endif" << CRLF;

		m_pListGeneratedFiles->append(szHeaderFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const TypeSharedPtr& pType)
{
	bool bQStringListIncluded = false;
	QString szCppFilename = pType->getQualifiedName() + ".cpp";
	QString szHeaderFilename = pType->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "types" + QDir::separator() + szCppFilename);
	if(file.exists()) {
		file.remove();
	}
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		os << "/*" << CRLF;
		os << " * " << szCppFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;

		if(pType->getClassType() == Type::TypeComplex){
			ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);
			if(pComplexType->getAttributeList()){
				if(pComplexType->getAttributeList()->containsListAttribute()){
					os << "#include <QStringList>" << CRLF;
					bQStringListIncluded = true;
				}
			}
			if(!bQStringListIncluded && pComplexType->getExtensionType() && pComplexType->isExtensionTypeList()){
				os << "#include <QStringList>" << CRLF;
				bQStringListIncluded = true;
			}

			if(pComplexType->getElementList()){
				ElementList::const_iterator iter;
				for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
					if((*iter)->getType() && (*iter)->isPointer()){
						os << "#include \"" << (*iter)->getType()->getQualifiedName() << ".h\"" << CRLF;
					}
				}
			}
		}

		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassType(os, pType);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pListGeneratedFiles->append(QString("types") + QDir::separator() + szCppFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const RequestResponseElementSharedPtr& pElement)
{
	QString szCppFilename = pElement->getQualifiedName() + ".cpp";
	QString szHeaderFilename = pElement->getQualifiedName() + ".h";

	QFile file(m_szDirname + QDir::separator() + "messages" + QDir::separator() + szCppFilename);
	if(file.exists()) {
		file.remove();
	}
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

		m_pListGeneratedFiles->append(QString("messages") + QDir::separator() + szCppFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildCppFile(const ServiceSharedPtr& pService)
{
	QString szCppFilename = pService->getName() + ".cpp";
	QString szHeaderFilename = pService->getName() + ".h";

	QFile file(m_szDirname + QDir::separator() + szCppFilename);
	if(file.exists()) {
		file.remove();
	}
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);

		os << "/*" << CRLF;
		os << " * " << szCppFilename << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#include <QUrl>" << CRLF;
		os << "#include <QEventLoop>" << CRLF;
		os << "#include <QDomElement>" << CRLF;
		os << "#include <QNetworkAccessManager>" << CRLF;
		os << "#include <QNetworkRequest>" << CRLF;
		os << "#include <QNetworkReply>" << CRLF;
		os << "#include <QStringList>" << CRLF;
		os << CRLF;
		os << "#include \"" << szHeaderFilename << "\"" << CRLF;
		os << CRLF;
		os << "#define TIMEOUT_MSEC 10*1000" << CRLF;
		os << CRLF;
		os << "#define SOAP_ENV_URI \"http://www.w3.org/2003/05/soap-envelope\"" << CRLF;
		os << "#define DEFAULT_SOAP_ENV_NAMESPACE \"SOAP-ENV\"" << CRLF;
		os << CRLF;

		if(!m_szNamespace.isEmpty()) {
			os << "namespace " << m_szNamespace << " {" << CRLF;
		}

		buildCppClassService(os, pService);

		if(!m_szNamespace.isEmpty()) {
			os << "}" << CRLF;
		}

		m_pListGeneratedFiles->append(szCppFilename);
	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szCppFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);

	os << "namespace " << StringUtils::secureString(pType->getNamespace().toUpper()) << " {" << CRLF;
	os << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		os << "class " << szClassname << CRLF;
		os << "{" << CRLF;
		os << "public:" << CRLF;

		if(pSimpleType->isEnumeration()) {
			os << "\t" << pSimpleType->getEnumerationDeclaration() << CRLF;
			os << CRLF;
		}

		os << "\t" << szClassname << "();" << CRLF;
		os << "\tvirtual ~" << szClassname << "();" << CRLF;
		os << CRLF;

		buildHeaderClassSimpleType(os, pSimpleType);

		os << "};" << CRLF;
		os << CRLF;

	} else if(pType->getClassType() == Type::TypeComplex) {
		ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pType);

		os << "const QString " << pComplexType->getLocalName(true) << "TargetNamespace = \"" << pComplexType->getNamespace() << "\";" CRLF;
		os << "const QString " << pComplexType->getLocalName(true) << "TargetNamespaceUri = \"" << pComplexType->getNamespaceUri() << "\";" CRLF;
		os << CRLF;

		if(pComplexType->getElementList()){
			ElementList::const_iterator iter;
			for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
				if((*iter)->getType() && (*iter)->isPointer()){
					os << "class " << (*iter)->getType()->getLocalName(true) << ";" << CRLF << CRLF;
				}
			}
		}

		if(pComplexType->getExtensionType().isNull()) {
			os << "class " << szClassname << CRLF;
		}else{
			QString szExtendedClassname;
			if(pComplexType->isExtensionTypeList()){
				szExtendedClassname = "QList<";
			}
			if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
				szExtendedClassname += pSimpleType->getVariableTypeString();
			}else{
				QString szExtensionName = pComplexType->getExtensionType()->getNameWithNamespace();
				szExtendedClassname += (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;
			}
			if(pComplexType->isExtensionTypeList()){
				szExtendedClassname += ">";
			}

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
	os << "\t" << pSimpleType->getDeserializerDeclaration() << CRLF;
	os << "\t" << pSimpleType->getEnumConvertDeclaration() << CRLF;
	os << "\t" << pSimpleType->getIsNullDeclaration() << CRLF;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\t" << pSimpleType->getVariableDeclaration() << CRLF;
}

void TypeListBuilder::buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		os << "public:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << "\t" << pAttribute->getSetterDeclaration();
			os << "\t" << pAttribute->getGetterDeclaration();
			os << CRLF;
		}

		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << "\t" << pElement->getSetterDeclaration() << CRLF ;
			os << "\t" << pElement->getGetterDeclaration() << CRLF ;
			os << CRLF;

		}
	}

	os << "\t" << pComplexType->getSerializerDeclaration() << CRLF;
	os << "\t" << pComplexType->getDeserializerDeclaration() << CRLF;
	os << CRLF;
	os << "\t" << pComplexType->getIsNullDeclaration() << CRLF;
	os << CRLF;
	os << "\t" << pComplexType->getGetNamespaceDeclarationDeclaration() << CRLF;
	os << CRLF;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {
		os << "private:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				continue;
			}

			os << "\t" << pAttribute->getVariableDeclaration() << CRLF;
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				continue;
			}

			os << "\t" << pElement->getVariableDeclaration() << CRLF ;
		}
	}
}

void TypeListBuilder::buildHeaderClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName(true);
	ComplexTypeSharedPtr pComplexType = pElement->getComplexType();

	os << "namespace " << StringUtils::secureString(pElement->getNamespace().toUpper()) << " {" << CRLF;
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

	os << "class " << szClassname << " : public Service" CRLF;
	os << "{" << CRLF;
	os << "public:" << CRLF;
	os << "\t" << szClassname << "();" << CRLF;
	os << "\tvirtual ~" << szClassname << "();" << CRLF;
	os << CRLF;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {
		if(!(*operation)->getInputMessage()) {
			continue;
		}
		os << "\t" << (*operation)->getOperationDeclaration() << CRLF;
	}

	os << "\tQMap<QString, QString> buildNamespaceRoutingMap(const QDomDocument& doc) const;" << CRLF;

	os << "};" << CRLF;
}

void TypeListBuilder::buildHeaderIncludeType(QTextStream& os, const TypeSharedPtr& pType) const
{
	os << "#include <QDomElement>" << CRLF;
	os << "#include <QList>" << CRLF;
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
		AttributeSharedPtr pAttribute;

		ElementListSharedPtr pListElements = pComplexType->getElementList();
		ElementList::const_iterator element;
		ElementSharedPtr pElement;

		if(!pComplexType->getExtensionType().isNull()) {
			QString szExtensionName;
			if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
				szExtensionName = pSimpleType->getVariableTypeFilenameString();
			}else{
				szExtensionName = pComplexType->getExtensionType()->getQualifiedName();
			}

			os << "#include \"" << szExtensionName << ".h\"" << CRLF;
		}

		if(pListAttributes->count() > 0 || pListElements->count() > 0) {

			QStringList list;

			for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
				if((*attr)->hasRef()){
					pAttribute = (*attr)->getRef();
				}else{
					pAttribute = *attr;
				}

				if(!pAttribute->getType()) {
					continue;
				}

				if( pAttribute->getType()->getClassType() == Type::TypeSimple) {
					SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());

					if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
						os << "#include \"" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
						list.append(pSimpleType->getVariableTypeFilenameString());

					}
				}else if( pAttribute->getType()->getClassType() == Type::TypeComplex) {
					ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());

					if(!list.contains(pComplexType->getQualifiedName())) {
						os << "#include \"" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
						list.append(pComplexType->getQualifiedName());

					}

				}
			}
			for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
				if((*element)->hasRef()){
					pElement = (*element)->getRef();
				}else{
					pElement = *element;
				}

				if(!pElement->getType() || pElement->isNested()){
					continue;
				}

				if(pElement->isPointer()){
					continue;
				}

				if( pElement->getType()->getClassType() == Type::TypeSimple) {
					SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());
					if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
						os << "#include \"" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
						list.append(pSimpleType->getVariableTypeFilenameString());

					}

				}else if(pElement->getType()->getClassType() == Type::TypeComplex) {
					ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
					if(!list.contains(pComplexType->getQualifiedName())) {
						os << "#include \"" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
						list.append(pComplexType->getQualifiedName());

					}

				}else{
					qWarning("[TypeListBuilder::buildHeaderIncludeType] Element %s has an unknown type: %s", qPrintable(pElement->getName()), qPrintable(pElement->getType()->getLocalName()));
					if(!list.contains(pElement->getType()->getQualifiedName())) {
						os << "#include \"" << pElement->getType()->getQualifiedName() << ".h\"" << CRLF;
						list.append(pElement->getType()->getQualifiedName());

					}
				}
			}
		}
	}
}

void TypeListBuilder::buildHeaderIncludeElement(QTextStream& os, const RequestResponseElementSharedPtr& pRequestResponseElement) const
{
	ComplexTypeSharedPtr pComplexType = pRequestResponseElement->getComplexType();

	os << "#include <QDomElement>" << CRLF;
	os << "#include <QList>" << CRLF;
	os << "#include <QString>" << CRLF;

	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(!pComplexType->getExtensionType().isNull()) {
		QString szExtensionName;
		if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
			SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
			szExtensionName = pSimpleType->getVariableTypeFilenameString();
		}else{
			szExtensionName = pComplexType->getExtensionType()->getQualifiedName();
		}

		os << "#include \"../types/" << szExtensionName << ".h\"" << CRLF;
	}

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		QStringList list;

		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				continue;
			}
			if( pAttribute->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pAttribute->getType());

				if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
					os << "#include \"../types/" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
					list.append(pSimpleType->getVariableTypeFilenameString());

				}
			}else if( pAttribute->getType()->getClassType() == Type::TypeComplex) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pAttribute->getType());

				if(!list.contains(pComplexType->getQualifiedName())) {
					os << "#include \"../types/" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
					list.append(pComplexType->getQualifiedName());

				}

			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			if(!pElement->getType()) {
				continue;
			}

			if(pElement->getType()->getClassType() == Type::TypeSimple) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pElement->getType());
				if(!list.contains(pSimpleType->getVariableTypeFilenameString())) {
					os << "#include \"../types/" << pSimpleType->getVariableTypeFilenameString() << ".h\"" << CRLF;
					list.append(pSimpleType->getVariableTypeFilenameString());

				}

			}else/* if( (*element)->getType()->getClassType() == Type::TypeComplex)*/ {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>(pElement->getType());
				if(!list.contains(pComplexType->getQualifiedName())) {
					os << "#include \"../types/" << pComplexType->getQualifiedName() << ".h\"" << CRLF;
					list.append(pComplexType->getQualifiedName());

				}

			}
		}
	}
}

void TypeListBuilder::buildHeaderIncludeService(QTextStream& os, const ServiceSharedPtr& pService) const
{
	QStringList list;
	bool bSoapEnvelopeFaultIncluded = false;

	os << "#include \"Service.h\"" << CRLF << CRLF;

	OperationListSharedPtr pOperationList = pService->getOperationList();
	OperationList::const_iterator operation;
	for(operation = pOperationList->constBegin(); operation != pOperationList->constEnd(); ++operation) {

		OperationSharedPtr pOperation = *operation;
		if(!pOperation->getInputMessage()) {
			continue;
		}
		//qDebug(qPrintable(pOperation->getName()));
		//qDebug(qPrintable(pOperation->getInputMessage() ? "NotNull" : "Null"));
		//qDebug(qPrintable(pOperation->getInputMessage()->getLocalName()));
		//qDebug(qPrintable(pOperation->getInputMessage()->getParameter() ? "NotNull" : "Null"));

		if(!bSoapEnvelopeFaultIncluded && pOperation->getSoapEnvelopeFaultType()){
			os << "#include \"types/" << (*operation)->getSoapEnvelopeFaultType()->getQualifiedName() << ".h\"" << CRLF;
			bSoapEnvelopeFaultIncluded = true;
		}

		if(!list.contains((*operation)->getInputMessage()->getParameter()->getQualifiedName())) {
			os << "#include \"messages/" << (*operation)->getInputMessage()->getParameter()->getQualifiedName() << ".h\"" << CRLF;
			list.append((*operation)->getInputMessage()->getParameter()->getQualifiedName());
		}
		if(!list.contains((*operation)->getOutputMessage()->getParameter()->getQualifiedName())) {
			os << "#include \"messages/" << (*operation)->getOutputMessage()->getParameter()->getQualifiedName() << ".h\"" << CRLF;
			list.append((*operation)->getOutputMessage()->getParameter()->getQualifiedName());
		}
	}

	os << CRLF;
}

void TypeListBuilder::buildCppClassType(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getLocalName(true);

	os << "namespace " << StringUtils::secureString(pType->getNamespace().toUpper()) << " {" << CRLF;
	os << CRLF;

	if(pType->getClassType() == Type::TypeSimple) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		if(pSimpleType->isEnumeration()) {

			os << szClassname << "::" << szClassname << "()" << CRLF;
			os << "{" << CRLF;
			os << "\t" << pSimpleType->getVariableName() << " = Unknown;" << CRLF;
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
			os << szClassname << "::" << szClassname << "() {";
			if(pComplexType->getElementList()){
				ElementList::const_iterator iter;
				for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
					if((*iter)->getMaxOccurs() == 1){
						if((*iter)->getType() && ((*iter)->isNested() || (*iter)->isPointer())){
							os << CRLF << "\t" << (*iter)->getVariableName() << " = NULL;" << CRLF;
						}
					}
				}
			}
			os << "}" << CRLF;

			os << szClassname << "::~" << szClassname << "() {";
			if(pComplexType->getElementList()){
				ElementList::const_iterator iter;
				for(iter = pComplexType->getElementList()->constBegin(); iter != pComplexType->getElementList()->constEnd(); ++iter){
					if((*iter)->getMaxOccurs() == 1){
						if((*iter)->getType() && ((*iter)->isNested() || (*iter)->isPointer())){
							os << CRLF << "\tif(" << (*iter)->getVariableName() << "){" << CRLF;
							os << "\t\tdelete " << (*iter)->getVariableName() << ";" << CRLF;
							os << "\t\t" << (*iter)->getVariableName() << " = NULL;" << CRLF;
							os << "\t}" << CRLF;
						}
					}
				}
			}
			os << "}" << CRLF;
			os << CRLF;
		}else{
			QString szExtensionName;
			if(pComplexType->isExtensionTypeList()){
				szExtensionName = "QList<";
			}
			if(pComplexType->getExtensionType()->getClassType() == Type::TypeSimple){
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pComplexType->getExtensionType());
				szExtensionName += pSimpleType->getVariableTypeString();
			}else{
				szExtensionName += pComplexType->getExtensionType()->getNameWithNamespace();
			}
			if(pComplexType->isExtensionTypeList()){
				szExtensionName += ">";
			}
			QString szExtendedClassname = (!m_szPrefix.isEmpty() ? m_szPrefix : "") + szExtensionName;

			os << szClassname << "::" << szClassname << "() :" << szExtendedClassname << "() {}" << CRLF;
			os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		}
		os << CRLF;

		buildCppClassComplexType(os, pComplexType, pComplexType->getNamespace());

		os << CRLF;
	}

	os << "}" << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pSimpleType->getLocalName();
	szClassname = StringUtils::secureString(szClassname);

	os << pSimpleType->getSetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getGetterDefinition(szClassname) << CRLF;
	os << pSimpleType->getSerializerDefinition(szClassname) << CRLF;
	os << pSimpleType->getDeserializerDefinition(szClassname) << CRLF;
	os << pSimpleType->getEnumConvertDefinition(szClassname) << CRLF;
	os << pSimpleType->getIsNullDefinition(szClassname) << CRLF;
	os << CRLF;
}

void TypeListBuilder::buildCppClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType, const QString& szTargetNamespace) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pComplexType->getLocalName();

	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;
	AttributeSharedPtr pAttribute;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;
	ElementSharedPtr pElement;

	if(pListAttributes->count() > 0 || pListElements->count() > 0) {

		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if((*attr)->hasRef()){
				pAttribute = (*attr)->getRef();
			}else{
				pAttribute = *attr;
			}

			if(!pAttribute->getType()) {
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable(pAttribute->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << pAttribute->getSetterDefinition(szClassname);
			os << pAttribute->getGetterDefinition(szClassname);
			os << CRLF;
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if((*element)->hasRef()){
				pElement = (*element)->getRef();
			}else{
				pElement = *element;
			}

			ComplexTypeSharedPtr pType = qSharedPointerCast<ComplexType>(pElement->getType());

			if(!pElement->getType()) {
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable(pElement->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}

			os << pElement->getSetterDefinition(szClassname) << CRLF ;
			os << pElement->getGetterDefinition(szClassname) << CRLF ;
			os << CRLF;
		}
	}
	os << pComplexType->getSerializerDefinition(szClassname, szTargetNamespace) << CRLF;
	os << pComplexType->getDeserializerDefinition(szClassname) << CRLF;
	os << CRLF;
	os << pComplexType->getIsNullDefinition(szClassname) << CRLF;
	os << pComplexType->getGetNamespaceDeclarationDefinition(szClassname) << CRLF;
}

void TypeListBuilder::buildCppClassElement(QTextStream& os, const RequestResponseElementSharedPtr& pElement) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pElement->getLocalName();
	ComplexTypeSharedPtr pComplexType = pElement->getComplexType();
	pComplexType->setLocalName(pElement->getLocalName());
	pComplexType->setNamespace(pElement->getNamespace());

	os << "namespace " << StringUtils::secureString(pElement->getNamespace().toUpper()) << " {" << CRLF;
	os << CRLF;

	if(!pComplexType.isNull()) {
		os << szClassname << "::" << szClassname << "() {}" << CRLF;
		os << szClassname << "::~" << szClassname << "() {}" << CRLF;
		os << CRLF;

		buildCppClassComplexType(os, pComplexType, pComplexType->getNamespace());

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
		if(!(*operation)->getInputMessage()) {
			continue;
		}
		os << (*operation)->getOperationDefinition(szClassname, m_szNamespace) << CRLF;
	}

	os << "QMap<QString, QString> " << szClassname << "::buildNamespaceRoutingMap(const QDomDocument& doc) const" CRLF;
	os << "{" CRLF;
	os << "\tQMap<QString, QString> map;" CRLF;
	os << "\tQString szSoapEnvNamespace = DEFAULT_SOAP_ENV_NAMESPACE;" CRLF;
	os << CRLF;
	os << "\t// Try to find the real soap envelope namespace" CRLF;
	os << "\tQDomNode node = doc.documentElement();" CRLF;
	os << "\twhile(!node.isNull()){" CRLF;
	os << "\t\tQDomElement elem = node.toElement();" CRLF;
	os << "\t\tif(!elem.isNull() && elem.tagName().endsWith(\":Envelope\")){" CRLF;
	os << "\t\t\tszSoapEnvNamespace = elem.tagName().split(\":\")[0];" CRLF;
	os << "\t\t}" CRLF;
	os << "\t\tnode = node.nextSibling();" CRLF;
	os << "\t}" CRLF;
	os << CRLF;
	os << "\tif(doc.elementsByTagName(szSoapEnvNamespace + \":Envelope\").size() > 0){" << CRLF;
	os << "\t\tQDomElement root = doc.elementsByTagName(szSoapEnvNamespace + \":Envelope\").at(0).toElement();" << CRLF;
	os << "\t\tint iNbAttributes = root.attributes().size();" << CRLF;
	os << "\t\tfor(int i = 0; i < iNbAttributes; ++i){" << CRLF;
	os << "\t\t\tQDomAttr attr = root.attributes().item(i).toAttr();" << CRLF;
	os << "\t\t\tQStringList nameList = attr.name().split(\":\");" << CRLF;
	os << "\t\t\tmap.insert(attr.value(), nameList[1]);" << CRLF;
	os << "\t\t}" CRLF;
	os << "\t}" CRLF;
	os << "\treturn map;" CRLF;
	os << "}" CRLF;
}
