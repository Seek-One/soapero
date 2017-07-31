# CMake file
cmake_minimum_required(VERSION "2.6")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Select build config
if (NOT DEFINED PRODUCT_CONFIG)
	set (PRODUCT_CONFIG "jet1oeil-soapero")
endif()

if (NOT DEFINED APPLICATION_VENDOR_NAME)
	set (APPLICATION_VENDOR_NAME "Jet1oeil")
endif()
if (NOT DEFINED APPLICATION_VENDOR_DOMAIN)
	set (APPLICATION_VENDOR_DOMAIN "www.jet1oeil.com")
endif()
if (NOT DEFINED APPLICATION_VENDOR_PACKAGE)
	set (APPLICATION_VENDOR_PACKAGE "jet1oeil")
endif()
if (NOT DEFINED APPLICATION_NAME)
	set (APPLICATION_NAME "Jet1oeil Soapero")
endif()
if (NOT DEFINED APPLICATION_PACKAGE_NAME)
	set (APPLICATION_PACKAGE_NAME "jet1oeil-soapero")
endif()
if (NOT DEFINED APPLICATION_EXE_NAME)
	set (APPLICATION_EXE_NAME "jet1oeil-soapero")
endif()

set (APPLICATION_VERSION_MAJOR 0)
set (APPLICATION_VERSION_MINOR 1)
set (APPLICATION_VERSION_PATCH 0)

if(NOT DEFINED APPLICATION_VERSION_BUILD)
	set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})

project (com.jet1oeil.monitor CXX C)

set (CMAKE_VERBOSE_MAKEFILE OFF)

if(WIN32)
	set(AUTO_NEWLINE_STYLE WIN32)
else()
	set(AUTO_NEWLINE_STYLE UNIX)
endif()
if(CMAKE_HOST_SYSTEM MATCHES Windows)
	set(HOST_WIN32 TRUE)
else()
	set(HOST_WIN32 FALSE)
endif()

# Define install paths
if(WIN32)
	set(INSTALL_PATH_BIN ".")
	set(INSTALL_PATH_SHARE ".")
	set(INSTALL_PATH_DOC ".")
	set(INSTALL_PATH_APPLICATIONS ".")
	set(INSTALL_PATH_ICONS ".")
else(WIN32)
	set(INSTALL_PATH_BIN "bin/")
	set(INSTALL_PATH_SHARE "share/${APPLICATION_PACKAGE_NAME}")
	set(INSTALL_PATH_DOC "share/${APPLICATION_PACKAGE_NAME}/doc")
	set(INSTALL_PATH_APPLICATIONS "share/applications")
	set(INSTALL_PATH_ICONS "share/icons")
endif(WIN32)

##################
# Macro
##################

include (CMakeModules/IncludeMacro.cmake)

##################
# Define options
##################

if(NOT MSVC)
	add_definitions (-Wall)
endif(NOT MSVC)

# Platform check from standard C
include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILES (stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (math.h HAVE_MATH_H)
CHECK_INCLUDE_FILES (regex.h HAVE_REGEX_H)
CHECK_INCLUDE_FILES (byteswap.h HAVE_BYTESWAP_H)
CHECK_INCLUDE_FILES (sys/time.h HAVE_SYS_TIME_H)

if (HAVE_STRING_H AND HAVE_STDLIB_H AND HAVE_STDIO_H AND HAVE_STDDEF_H AND HAVE_MATH_H)
  message(STATUS "All required headers are found")
else()
  message(FATAL_ERROR "Some required headers are not found")
endif()

include (CheckFunctionExists)
CHECK_FUNCTION_EXISTS (strdup HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (strcasestr HAVE_STRCASESTR)
CHECK_FUNCTION_EXISTS (strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS (strncasecmp HAVE_STRNCASECMP)
CHECK_FUNCTION_EXISTS (_stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS (_strnicmp HAVE_STRNICMP)
CHECK_FUNCTION_EXISTS (strndup HAVE_STRNDUP)
CHECK_FUNCTION_EXISTS (strtok_r HAVE_STRTOK_R)
CHECK_FUNCTION_EXISTS (abs HAVE_ABS)
#CHECK_FUNCTION_EXISTS (fabs HAVE_FABS)

if (HAVE_STRDUP AND (HAVE_STRCASESTR OR HAVE_STRICMP) AND (HAVE_STRNCASECMP OR HAVE_STRNICMP) AND HAVE_ABS)
  message(STATUS "All required functions are found")
else()
  message(FATAL_ERROR "Some required functions are not found")
endif()

####################
# Library checking
####################

# Compiler
set(WITH_COMPILER_LIBRARIES ON CACHE BOOL "Enable build with compiler library")
# GUI
set(WITH_QT ON CACHE BOOL "Enable build with qt library")
set(QT4_COMPONENTS QtCore QtXml)
set(QT5_COMPONENTS Core Xml)

# Define output variable
set (PACKAGE_DEPS)

list(APPEND PACKAGE_DEPS "consolekit")

include (CMakeModules/IncludeLibraries.cmake)


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
set (APP_SOURCES
#	generated/doorcontrol.cpp
#	generated/doorcontrol.h

	src/Base/XSBoolean.cpp
	src/Base/XSBoolean.h
	src/Base/XSDuration.cpp
	src/Base/XSDuration.h
	src/Base/XSInteger.cpp
	src/Base/XSInteger.h
	src/Base/XSString.cpp
	src/Base/XSString.h
	src/Base/XSUnsignedInteger.cpp
	src/Base/XSUnsignedInteger.h

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
	src/Parser/QWSDLParserHandler.h
	src/Parser/QWSDLParserHandler.cpp
)

# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(MOC_HEADERS
)
QT_WRAP_CPP(MOC_OUTFILES ${MOC_HEADERS})

####################
# i18n
####################

set (I18N_FILES
)

set (TS_FILES
    ts/fr.ts
)

option (UPDATE_TRANSLATIONS "Update source translation ts/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)" ON)
if (UPDATE_TRANSLATIONS)
  qt_create_translation(qm_FILES ${TS_FILES} ${I18N_FILES})
else (UPDATE_TRANSLATIONS)
  qt_add_translation(qm_FILES ${TS_FILES})
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
set (QRC_FILES
	${PROJECT_BINARY_DIR}/ts.qrc
)
QT_ADD_RESOURCES(RCS_OUTFILES ${QRC_FILES})

####################
# Create Executable
####################

SET (SOAPERO_MAIN_SRC 
    src/main.cpp
)

# Build executable
add_executable (${APPLICATION_EXE_NAME}
	${GUI_TYPE}
	${RC_FILES}
	${SOAPERO_MAIN_SRC}
	${APP_SOURCES}
	${RCS_OUTFILES}
	${MOC_OUTFILES}
	${qm_FILES}
)
target_link_libraries (
	${APPLICATION_EXE_NAME}
	${QT_LIBRARIES} 
	${QT_QTMAIN_LIBRARY}
)
install(TARGETS ${APPLICATION_EXE_NAME} DESTINATION ${INSTALL_PATH_BIN})

# README file
configure_file (
  "${PROJECT_SOURCE_DIR}/README.in"
  "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}"
  NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# COPYING file
configure_file (
  "${PROJECT_SOURCE_DIR}/COPYING.in"
  "${PROJECT_BINARY_DIR}/COPYING${DOCFILES_EXT}"
  NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
# ChangeLog file
set(CHANGELOG "ChangeLog")
install(FILES "${PROJECT_SOURCE_DIR}/${CHANGELOG}" DESTINATION ${INSTALL_PATH_DOC} RENAME ChangeLog${DOCFILES_EXT})


