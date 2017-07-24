/*
 * SimpleType.h
 *
 *  Created on: 19 juil. 2017
 *      Author: lgruber
 */

#ifndef SIMPLETYPE_H_
#define SIMPLETYPE_H_

#include <QSharedPointer>
#include <QList>
#include <QString>
#include <QStringList>

#include "Type.h"

class SimpleType : public Type
{
public:
	enum VariableType {
		Unknown,
		string,
		unsignedInt,
		Boolean,
	};

	SimpleType();
	virtual ~SimpleType();

	static SimpleTypeSharedPtr create();

	void setVariableType(VariableType type);
	void setVariableTypeFromString(const QString& szType);
	VariableType getVariableType()const;
	QString getVariableTypeString()const;

	QString getSetterDeclaration() const;
	QString getGetterDeclaration() const;
	QString getVariableDeclaration() const;
	QString getEnumerationDeclaration() const;

	void setRestricted(bool bRestricted);
	bool isRestricted() const;

	void addEnumerationValue(const QString& szValue);
	QStringList getEnumerationValues() const;

	void setMaxLength(int iMaxLength);
	int getMaxLength()const;

	void setMinLength(int iMinLength);
	int getMinLength()const;

	void setMinInclusive(int iMinInclusive);
	int getMinInclusive()const;

private:
	VariableType m_variableType;

	//For base string
	int m_iMaxLength;
	int m_iMinLength;

	//For base unsigned int
	int m_iMinInclusive;

	bool m_bRestricted;
	QStringList m_listEnumerationValues;

};

class SimpleTypeList : public QList<SimpleTypeSharedPtr>
{
public:
	SimpleTypeList();
	virtual ~SimpleTypeList();

	static SimpleTypeListSharedPtr create();
};




#endif /* SIMPLETYPE_H_ */
