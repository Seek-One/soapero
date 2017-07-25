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

	void setName(const QString& szName);
	QString getName() const;

	void setComplexType(const ComplexTypeSharedPtr& pComplexType);
	ComplexTypeSharedPtr getComplexType() const;

private:
	QString m_szName;

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
