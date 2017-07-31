/*
 * Message.cpp
 *
 *  Created on: 25 juil. 2017
 *      Author: lgruber
 */

#include "Message.h"

Message::Message()
	:Classname()
{

}

Message::~Message()
{

}

MessageSharedPtr Message::create()
{
	return MessageSharedPtr(new Message());
}

void Message::setParameter(const RequestResponseElementSharedPtr& pParameter)
{
	m_pParameter = pParameter;
}

RequestResponseElementSharedPtr Message::getParameter() const
{
	return m_pParameter;
}

MessageList::MessageList()
	:QList<MessageSharedPtr>()
{

}

MessageList::~MessageList()
{

}

MessageListSharedPtr MessageList::create()
{
	return MessageListSharedPtr(new MessageList());
}

MessageSharedPtr MessageList::getByName(const QString& szLocalName, const QString& szNamespace)
{
	MessageList::const_iterator message;
	for(message = constBegin(); message != constEnd(); ++message) {
		if( (*message)->getLocalName() == szLocalName &&
				(*message)->getNamespace() == szNamespace) {
			return *message;
		}
	}
	return MessageSharedPtr();
}
