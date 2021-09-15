//
// Created by ebeuque on 15/09/2021.
//

#ifndef COM_JET1OEIL_SOAPERO_QWSDLDATA_H
#define COM_JET1OEIL_SOAPERO_QWSDLDATA_H

#include <QList>

class QWSDLData
{
public:
	QWSDLData();
	virtual ~QWSDLData();

	// Loaded URI
	void addLoadedURI(const QString& szURI);
	bool hasLoadedURI(const QString& szURI) const;

private:
	QList<QString> m_listLoadedURI;
};


#endif //COM_JET1OEIL_SOAPERO_QWSDLDATA_H
