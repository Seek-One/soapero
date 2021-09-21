soapero
============

soapero is a simple client generator for SOAP service from WSDL.

- Github: https://github.com/Jet1oeil/soapero
- Developer website: http://www.jet1oeil.com/
- Bugs reporting: https://github.com/Jet1oeil/soapero/issues

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
- Qt version 4 or Qt version 5

Compiling from source
---------------------

Install dependencies:
  * in debian OS (with Qt4) :
```
    apt-get install git cmake libqt4-dev
```
  * in debian OS (with Qt5) :
```
    apt-get install git cmake qtbase5-dev qttools5-dev qttools5-dev-tools
```
  * in fedora OS : 
```
    dnf install cmake gcc-c++ qt5-devel
```
Get the code:

    git clone https://github.com/Jet1oeil/soapero.git && cd soapero

Compile:

    ./build.sh regen
    make

Run:

    ./soapero SRC_DIR DST_DIR --namespace=MYNAMESPACE

Options:

	--namespace=[NAMESPACE]: Global namespace to use for generated class (Mandatory)
	--output-mode=[OUTPUT_MODE]: "Default" or "CMakeLists"
	--resources-dir=[DIR]: Path where to find resources files. Used to copy some files in output directory. (Default: ./resources)


