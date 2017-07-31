/*
 * Message.h
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <QSharedPointer>

#include "Classname.h"
#include "RequestResponseElement.h"

class Message;
typedef QSharedPointer<Message> MessageSharedPtr;
class MessageList;
typedef QSharedPointer<MessageList> MessageListSharedPtr;

class Message : public Classname
{
public:
	Message();
	virtual ~Message();

	static MessageSharedPtr create();

	void setParameter(const RequestResponseElementSharedPtr& pParameter);
	RequestResponseElementSharedPtr getParameter() const;

private:
	RequestResponseElementSharedPtr m_pParameter;
};

class MessageList : public QList<MessageSharedPtr>
{
public:
	MessageList();
	virtual ~MessageList();

	static MessageListSharedPtr create();

	MessageSharedPtr getByName(const QString& szLocalName, const QString& szNamespace);

};

#endif /* MESSAGE_H_ */
