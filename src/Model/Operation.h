/*
 * Operation.h
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#ifndef OPERATION_H_
#define OPERATION_H_

#include <QSharedPointer>

#include "Message.h"

class Operation;
typedef QSharedPointer<Operation> OperationSharedPtr;
class OperationList;
typedef QSharedPointer<OperationList> OperationListSharedPtr;


class Operation
{
public:
	Operation();
	virtual ~Operation();

	static OperationSharedPtr create();

	void setName(const QString& szName);
	QString getName() const;

	void setInputMessage(const MessageSharedPtr& pMessage);
	MessageSharedPtr getInputMessage() const;

	void setOutputMessage(const MessageSharedPtr& pMessage);
	MessageSharedPtr getOutputMessage() const;

	QString getOperationDeclaration() const;
	QString getOperationDefinition(const QString& szClassname) const;

private:
	QString m_szName;

	MessageSharedPtr m_pInputMessage;
	MessageSharedPtr m_pOutputMessage;
};

class OperationList : public QList<OperationSharedPtr>
{
public:
	OperationList();
	virtual ~OperationList();

	static OperationListSharedPtr create();

	OperationSharedPtr getByName(const QString& szName);

};

#endif /* OPERATION_H_ */
