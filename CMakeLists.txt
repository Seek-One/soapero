# CMake file

cmake_minimum_required(VERSION "3.13.4")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Define project data
set (APPLICATION_VENDOR_NAME "seekone")
set (APPLICATION_VENDOR_DOMAIN "www.seek-one.fr")
set (APPLICATION_NAME "soapero")
set (APPLICATION_VERSION_MAJOR 0)
set (APPLICATION_VERSION_MINOR 2)
set (APPLICATION_VERSION_PATCH 0)
if(NOT DEFINED APPLICATION_VERSION_BUILD)
	set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
set (APPLICATION_CONTACTMAIL "contact@seek-one.fr")
set (APPLICATION_CONTACT "Contact Seek-One <${APPLICATION_CONTACTMAIL}>")
set (APPLICATION_PACKAGE_VENDOR "seekone")
set (APPLICATION_PACKAGE_NAME "soapero")
set (APPLICATION_EXE_NAME soapero)

project (soapero CXX C)

set (CMAKE_VERBOSE_MAKEFILE OFF)

##############################
# Project configuration
##############################

# Define common project config
include (CMakeModules/ProjectCommonConfig.cmake)


##################
# Define options
##################

include (CMakeModules/ProjectCompilerChecks.cmake)

####################
# Library checking
####################

# Compiler
set(WITH_COMPILER_LIBRARIES ON CACHE BOOL "Enable build with compiler libraries")
# GUI
set(WITH_QT ON CACHE BOOL "Enable build with qt library")
set(QT4_COMPONENTS QtCore QtNetwork QtXml)
set(QT5_COMPONENTS Core Network Xml)
set(QT6_COMPONENTS Core Network Xml)

# Define output variable
set (PACKAGE_DEPS)
if(WITH_AUTONOMOUS_PACKAGE_BUILD)
	set(AUTONOMOUS_PACKAGE_BUILD ON CACHE BOOL "Enable build of autonomous package without dependencies")
endif()
list(APPEND PACKAGE_DEPS "consolekit")

include (CMakeModules/ProjectIncludeLibraries.cmake)

####################
# Config file
####################

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)
configure_file (
	"${PROJECT_SOURCE_DIR}/version.h.in"
	"${PROJECT_BINARY_DIR}/version.h"
)
add_definitions(-DHAVE_VERSION_H)
include_directories (${PROJECT_BINARY_DIR})

####################
# Add sources
####################

include_directories (${PROJECT_SOURCE_DIR}/src)

# src directory
set(APPLICATION_SOURCES
	resources/Base/xs/types/AnySimpleType.cpp
	resources/Base/xs/types/AnySimpleType.h
	resources/Base/xs/types/AnyType.cpp
	resources/Base/xs/types/AnyType.h
	resources/Base/xs/types/AnyURI.cpp
	resources/Base/xs/types/AnyURI.h
	resources/Base/xs/types/Base64Binary.cpp
	resources/Base/xs/types/Base64Binary.h
	resources/Base/xs/types/Boolean.cpp
	resources/Base/xs/types/Boolean.h
	resources/Base/xs/types/DateTime.cpp
	resources/Base/xs/types/DateTime.h
	resources/Base/xs/types/Double.cpp
	resources/Base/xs/types/Double.h
	resources/Base/xs/types/Duration.cpp
	resources/Base/xs/types/Duration.h
	resources/Base/xs/types/Float.cpp
	resources/Base/xs/types/Float.h
	resources/Base/xs/types/HexBinary.cpp
	resources/Base/xs/types/HexBinary.h
	resources/Base/xs/types/Integer.cpp
	resources/Base/xs/types/Integer.h
	resources/Base/xs/types/NCName.cpp
	resources/Base/xs/types/NCName.h
	resources/Base/xs/types/NonNegativeInteger.cpp
	resources/Base/xs/types/NonNegativeInteger.h
	resources/Base/xs/types/QName.cpp
	resources/Base/xs/types/QName.h
	resources/Base/xs/types/String.cpp
	resources/Base/xs/types/String.h
	resources/Base/xs/types/Token.cpp
	resources/Base/xs/types/Token.h
	resources/Base/xs/types/UnsignedInteger.cpp
	resources/Base/xs/types/UnsignedInteger.h
	resources/Base/xs/types/UnsignedLong.cpp
	resources/Base/xs/types/UnsignedLong.h

	resources/Service/Service.h
	resources/Service/Service.cpp

	src/Builder/FileBuilder.cpp
	src/Builder/FileBuilder.h
	src/Builder/FileHelper.cpp
	src/Builder/FileHelper.h
	src/Builder/TypeListBuilder.cpp
	src/Builder/TypeListBuilder.h

	src/Model/Classname.h
	src/Model/Classname.cpp
	src/Model/ComplexType.h
	src/Model/ComplexType.cpp
	src/Model/Message.h
	src/Model/Message.cpp
	src/Model/Operation.h
	src/Model/Operation.cpp
	src/Model/RequestResponseElement.h
	src/Model/RequestResponseElement.cpp
	src/Model/Service.h
	src/Model/Service.cpp
	src/Model/SimpleType.h
	src/Model/SimpleType.cpp
	src/Model/Type.h
	src/Model/Type.cpp
	src/Model/TypeRef.h
	src/Model/TypeRef.cpp

	src/Parser/QWSDLData.h
	src/Parser/QWSDLData.cpp
	src/Parser/QWSDLParser.h
	src/Parser/QWSDLParser.cpp
	src/Parser/WSDLAttributes.h
	src/Parser/WSDLSections.h

	src/Utils/ModelUtils.h
	src/Utils/ModelUtils.cpp
	src/Utils/StringUtils.h
	src/Utils/UniqueStringList.cpp
	src/Utils/UniqueStringList.h
)

# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(APPLICATION_MOC_HEADERS
)
qtx_wrap_cpp(
	APPLICATION_MOC_OUTFILES
	${APPLICATION_MOC_HEADERS}
)

####################
# i18n
####################

set (I18N_FILES
	src/main.cpp
)

set (TS_FILES
	ts/fr.ts
)

option (UPDATE_TRANSLATIONS "Update source translation ts/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)" ON)
if (UPDATE_TRANSLATIONS)
	qtx_create_translation(qm_FILES ${TS_FILES} ${I18N_FILES})
else (UPDATE_TRANSLATIONS)
	qtx_add_translation(qm_FILES ${TS_FILES})
endif (UPDATE_TRANSLATIONS)

add_custom_target(update_ts ALL DEPENDS ${qm_FILES})

####################
# Resource files
####################

# Configure translation file
configure_file (
	"${PROJECT_SOURCE_DIR}/ts.qrc.in"
	"${PROJECT_BINARY_DIR}/ts.qrc"
)

# Resources
set (QRC_TS_FILES
	${PROJECT_BINARY_DIR}/ts.qrc
)
qtx_add_resources(RCS_OUTFILES ${QRC_FILES})
qtx_add_resources(RCS_TS_OUTFILES ${QRC_TS_FILES})

####################
# Create Executable
####################

set (SOAPERO_MAIN_SRC
	src/main.cpp
)

# Build executable
add_executable (${APPLICATION_EXE_NAME}
	${GUI_TYPE}
	${RC_FILES}
	${SOAPERO_MAIN_SRC}
	${APPLICATION_SOURCES}
	${APPLICATION_MOC_OUTFILES}
	${RCS_OUTFILES}
	${RCS_TS_OUTFILES}
	${qm_FILES}
)
target_link_libraries (${APPLICATION_EXE_NAME}
	${QT_LIBRARIES}
	${QT_QTMAIN_LIBRARY}
)
install(TARGETS ${APPLICATION_EXE_NAME} DESTINATION ${INSTALL_PATH_BIN})

############################
# Common files installation
############################

# Documentation file
set(DOCFILES_EXT "")
if(WIN32)
	set(DOCFILES_EXT ".txt")
endif(WIN32)

# README file
configure_file (
	"${PROJECT_SOURCE_DIR}/README.md"
	"${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}"
	NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# COPYING file
configure_file (
	"${PROJECT_SOURCE_DIR}/COPYING"
	"${PROJECT_BINARY_DIR}/COPYING${DOCFILES_EXT}"
	NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
# ChangeLog file
set(CHANGELOG "ChangeLog")
install(FILES "${PROJECT_SOURCE_DIR}/${CHANGELOG}" DESTINATION ${INSTALL_PATH_DOC} RENAME ChangeLog${DOCFILES_EXT})
