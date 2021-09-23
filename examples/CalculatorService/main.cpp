#include <QCoreApplication>

#include "calculator/CalculatorService.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	
	qDebug("Starting application");
/*
	calc::CalcService service;
	service.setBaseUrl(QUrl("https://ecs.syr.edu/faculty/fawcett/handouts/cse686/code/calcWebService/"));

	XS::Integer iA;
	iA.setValue(1);
	XS::Integer iB;
	iB.setValue(2);

	calc::TNS::Add requestAdd;
	requestAdd.setA(iA);
	requestAdd.setB(iB);

	calc::TNS::AddResponse requestResponse;		

	service.Add(requestAdd, requestResponse);

curl --location --request POST 'http://www.dneonline.com/calculator.asmx' \
--header 'Content-Type: text/xml; charset=utf-8' \
--header 'SOAPAction: http://tempuri.org/Divide' \
--data-raw '<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <Divide xmlns="http://tempuri.org/">
      <intA>5</intA>
      <intB>5</intB>
    </Divide>
  </soap:Body>
</soap:Envelope>'

	*/

	calculator::CalculatorService service;
	service.setBaseUrl(QUrl("http://www.dneonline.com/calculator.asmx"));

	XS::Integer iA;
	iA.setValue(1);
	XS::Integer iB;
	iB.setValue(2);

	calculator::TNS::Add request;
	request.setIntA(iA);
	request.setIntB(iB);

	calculator::TNS::AddResponse response;		
	service.Add(request, response);

	qDebug("Response: %d", response.getAddResult().getValue());


	qDebug("Exit application");


	return 0;
}
