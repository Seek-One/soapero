soapero
============

soapero is a simple client generator for SOAP service from WSDL.

- Github: https://github.com/Seek-One/soapero
- Developer website: http://www.seek-one.fr/
- Bugs reporting: https://github.com/Seek-One/soapero/issues

Features
--------
- Support for WSDL and XSD parsing recursively
- Support for namespaces
- Support for WSUserNameToken authentication
- Automatical downloads for dependencies

License
-------

This program is licensed under the terms of the GNU GENERAL PUBLIC LICENSE Version 3.

All generated files are licensed under the terms of the MIT License.

Requirements
------------

- CMake
- Qt version 5 or Qt version 6

Compiling from source
---------------------

Install dependencies:
  * in debian OS (with Qt5) :
```
    apt-get install git cmake qtbase5-dev qttools5-dev qttools5-dev-tools
```
  * in debian OS (with Qt6) :
```
    apt-get install git cmake qt6-base-dev qt6-base-private-dev qt6-tools-dev qt6-base-dev-tools
```
  * in fedora OS : 
```
    dnf install cmake gcc-c++ qt5-devel
```
Get the code:

    git clone https://github.com/Seek-One/soapero.git && cd soapero

Compile:

    ./build.sh regen
    make

Run:

    ./soapero SRC_DIR DST_DIR --namespace=MYNAMESPACE

Options:

	--namespace=[NAMESPACE]: Global namespace to use for generated class (Mandatory)
	--output-mode=[OUTPUT_MODE]: "Default" or "CMakeLists"
	--resources-dir=[DIR]: Path where to find resources files. Used to copy some files in output directory. (Default: ./resources)
	--service-name=[SERVICE_NAME]: Force service name, useful for wsdl service not defining the name tag.


Examples
--------

Check the Calculator example: https://github.com/Seek-One/soapero/tree/master/examples/CalculatorService


