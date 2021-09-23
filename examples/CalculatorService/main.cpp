#include <QCoreApplication>

#include "calculator/CalculatorService.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	
	qDebug("Starting application");

	if(argc<3){
		qDebug("Usage: A B");
		return 0;
	}

	QString szArg1 = argv[1];
	QString szArg2 = argv[2];

	calculator::CalculatorService service;
	service.setBaseUrl(QUrl("http://www.dneonline.com/calculator.asmx"));
	service.setDebug(true);

	XS::Integer iA;
	iA.setValue(szArg1.toInt());
	XS::Integer iB;
	iB.setValue(szArg2.toInt());

	qDebug("Compute %d + %d:", iA.getValue(), iB.getValue());

	calculator::TNS::Add request;
	request.setIntA(iA);
	request.setIntB(iB);

	calculator::TNS::AddResponse response;		
	service.Add(request, response);

	qDebug("Response: %d", response.getAddResult().getValue());


	qDebug("Exit application");


	return 0;
}
