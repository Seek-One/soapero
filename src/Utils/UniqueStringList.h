/*
 * UniqueStringList.h
 *
 *  Created on: 3 sept. 2018
 *      Author: alavier
 */

#ifndef SRC_UTILS_UNIQUESTRINGLIST_H_
#define SRC_UTILS_UNIQUESTRINGLIST_H_

#include <QList>
#include <QString>

class UniqueStringList : public QList<QString> {
public:
	UniqueStringList();
	virtual ~UniqueStringList();

    void append(const QString& szValue);
    void append(const UniqueStringList& list);
};

#endif /* SRC_UTILS_UNIQUESTRINGLIST_H_ */
