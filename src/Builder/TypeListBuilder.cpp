/*
 * TypeListBuilder.cpp
 *
 *  Created on: 17 juil. 2017
 *      Author: lgruber
 */

#include <QDateTime>
#include <QCoreApplication>
#include <QStringList>

#include "TypeListBuilder.h"

#ifndef CRLF
#define CRLF "\r\n"
#endif

TypeListBuilder::TypeListBuilder(const TypeListSharedPtr& type)
{
	m_pListType = type;
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

void TypeListBuilder::setFilename(const QString& szFilename)
{
	m_szFilename = szFilename;
}

QString TypeListBuilder::getFilename() const
{
	return m_szFilename;
}

void TypeListBuilder::buildHeaderFile()
{
	if(m_szFilename.isEmpty()) {
		qWarning("[TypeListBuilder] Filename for output not set");
		return;
	}

	TypeList::const_iterator type;

	QString szHeaderFilename = m_szFilename + ".h";

	QFile file(szHeaderFilename);
	if(file.open(QFile::WriteOnly)) {

		QTextStream os(&file);
		QString szDefine = (!m_szPrefix.isEmpty() ? m_szPrefix.toUpper() + "_" : "") + m_szFilename.toUpper() + "_H_";

		os << "/*" << CRLF;
		os << " * " << m_szFilename << ".h" << CRLF;
		os << " * Created on: " << QDateTime::currentDateTime().toString("dd MMM yyyy") << CRLF;
		os << " *     Author: " << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion() << CRLF;
		os << " */" << CRLF;
		os << CRLF;
		os << "#ifndef " << szDefine << CRLF;
		os << "#define " << szDefine << CRLF;
		os << CRLF;

		for(type = m_pListType->constBegin(); type != m_pListType->constEnd(); ++type) {

			if(!(*type)->getLocalName().isEmpty()) {
				buildHeaderClass(os, *type);
			}

		}

		os << "#endif" << CRLF;

	}else{
		qWarning("[TypeListBuilder] Cannot open file %s (error: %s)", qPrintable(szHeaderFilename), qPrintable(file.errorString()));
	}
}

void TypeListBuilder::buildHeaderClass(QTextStream& os, const TypeSharedPtr& pType) const
{
	QString szClassname =  (!m_szPrefix.isEmpty() ? m_szPrefix : "") + pType->getQualifiedName();

	if(pType->getClassType() == Type::SimpleType) {
		SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>(pType);

		os << "class " << szClassname << CRLF;
		os << "{" << CRLF;
		os << "public:" << CRLF;

		if(pSimpleType->isRestricted()) {
			if(pSimpleType->getVariableType() == SimpleType::string &&
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

	} else if(pType->getClassType() == Type::ComplexType) {
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
}

void TypeListBuilder::buildHeaderClassSimpleType(QTextStream& os, const SimpleTypeSharedPtr& pSimpleType) const
{

	os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
	os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
	os << CRLF;
	os << "private:" << CRLF;
	os << "\t" << pSimpleType->getVariableDeclaration() << CRLF;
}

void TypeListBuilder::buildHeaderClassComplexType(QTextStream& os, const ComplexTypeSharedPtr& pComplexType) const
{
	QString szClassname = (m_szPrefix.isEmpty() ? "" : m_szPrefix) + pComplexType->getQualifiedName();
	AttributeListSharedPtr pListAttributes = pComplexType->getAttributeList();
	AttributeList::const_iterator attr;

	ElementListSharedPtr pListElements = pComplexType->getElementList();
	ElementList::const_iterator element;

	if(pListAttributes->count() > 0 || pListElements->count()) {

		os << "public:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if(!(*attr)->getType()) {
				qWarning("[TypeListBuilder] Attribute %s in %s has no type", qPrintable((*attr)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*attr)->getType()->getClassType() == Type::SimpleType) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());

				os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
				os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
				os << CRLF;

			}else if( (*attr)->getType()->getClassType() == Type::ComplexType) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());

				os << "\t" << pComplexType->getSetterDeclaration() << CRLF ;
				os << "\t" << pComplexType->getGetterDeclaration() << CRLF ;
				os << CRLF;
			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {
			if(!(*element)->getType()) {
				qWarning("[TypeListBuilder] Element %s in %s has no type", qPrintable((*element)->getName()), qPrintable(pComplexType->getQualifiedName()));
				continue;
			}
			if( (*element)->getType()->getClassType() == Type::SimpleType) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());

				os << "\t" << pSimpleType->getSetterDeclaration() << CRLF ;
				os << "\t" << pSimpleType->getGetterDeclaration() << CRLF ;
				os << CRLF;

			}else if( (*element)->getType()->getClassType() == Type::ComplexType) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());

				os << "\t" << pComplexType->getSetterDeclaration() << CRLF ;
				os << "\t" << pComplexType->getGetterDeclaration() << CRLF ;
				os << CRLF;
			}
		}


		os << "private:" << CRLF;
		for(attr = pListAttributes->constBegin(); attr != pListAttributes->constEnd(); ++attr) {
			if(!(*attr)->getType()) {
				continue;
			}
			if( (*attr)->getType()->getClassType() == Type::SimpleType) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*attr)->getType());
				os << "\t" << pSimpleType->getVariableDeclaration() << CRLF ;

			}else if( (*attr)->getType()->getClassType() == Type::ComplexType) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*attr)->getType());
				os << "\t" << pComplexType->getVariableDeclaration() << CRLF ;


			}
		}
		for(element = pListElements->constBegin(); element != pListElements->constEnd(); ++element) {

			if(!(*element)->getType()) {
				continue;
			}

			if( (*element)->getType()->getClassType() == Type::SimpleType) {
				SimpleTypeSharedPtr pSimpleType = qSharedPointerCast<SimpleType>((*element)->getType());
				os << "\t" << pSimpleType->getVariableDeclaration() << CRLF ;

			}else if( (*element)->getType()->getClassType() == Type::ComplexType) {
				ComplexTypeSharedPtr pComplexType = qSharedPointerCast<ComplexType>((*element)->getType());
				os << "\t" << pComplexType->getVariableDeclaration() << CRLF ;

			}
		}
	}
}














