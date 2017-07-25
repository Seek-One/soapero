/*
 * RequestResponseElement.h
 *
 *  Created on: 24 juil. 2017
 *      Author: lgruber
 */

#ifndef REQUESTRESPONSEELEMENT_H_
#define REQUESTRESPONSEELEMENT_H_

#include <QList>

#include "ComplexType.h"

class RequestResponseElement;
typedef QSharedPointer<RequestResponseElement> RequestResponseElementSharedPtr;
class RequestResponseElementList;
typedef QSharedPointer<RequestResponseElementList> RequestResponseElementListSharedPtr;

class RequestResponseElement
{
public:
	RequestResponseElement();
	virtual ~RequestResponseElement();

	static RequestResponseElementSharedPtr create();


	void setNamespace(const QString& szNamespace);
	void setLocalName(const QString& szLocalName);
	QString getQualifedName() const;
	QString getNamespace() const;
	QString getLocalName() const;

	void setComplexType(const ComplexTypeSharedPtr& pComplexType);
	ComplexTypeSharedPtr getComplexType() const;

private:
	QString m_szLocalName;
	QString m_szNamespace;

	ComplexTypeSharedPtr m_pComplexType;

};

class RequestResponseElementList : public QList<RequestResponseElementSharedPtr>
{
public:
	RequestResponseElementList();
	virtual ~RequestResponseElementList();

	static RequestResponseElementListSharedPtr create();
};

#endif /* REQUESTRESPONSEELEMENT_H_ */
