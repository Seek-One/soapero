echo "Test divide:"

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

echo "Test add:"

curl --location --request POST 'http://www.dneonline.com/calculator.asmx' \
--header 'Content-Type: text/xml; charset=utf-8' \
--header 'SOAPAction: http://tempuri.org/Add' \
--data-raw '<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <Add xmlns="http://tempuri.org/">
      <intA>5</intA>
      <intB>5</intB>
    </Add>
  </soap:Body>
</soap:Envelope>'
