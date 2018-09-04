/*
 * UniqueStringList.cpp
 *
 *  Created on: 3 sept. 2018
 *      Author: alavier
 */

#include "UniqueStringList.h"

UniqueStringList::UniqueStringList()
{

}

UniqueStringList::~UniqueStringList()
{

}

void UniqueStringList::append(const QString& szValue)
{
	if(!contains(szValue)){
		QList::append(szValue);
	}
}

void UniqueStringList::append(const UniqueStringList& list)
{
	UniqueStringList::const_iterator iter;
	for(iter = list.constBegin(); iter != list.constEnd(); ++iter){
		append(*iter);
	}
}
