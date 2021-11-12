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
set (APPLICATION_VERSION_MINOR 2)
set (APPLICATION_VERSION_PATCH 0)

if(NOT DEFINED APPLICATION_VERSION_BUILD)
    set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})

project (com.jet1oeil.soapero CXX C)

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
    set(INSTALL_PATH_SHARE ".")
    set(INSTALL_PATH_BIN ".")
    set(INSTALL_PATH_LIB ".")
    set(INSTALL_PATH_PLUGIN ".")
    set(INSTALL_PATH_DOC ".")
    set(INSTALL_PATH_APPLICATIONS ".")
    set(INSTALL_PATH_ICONS ".")
elseif(APPLE)
    set(INSTALL_PATH_SHARE ".")
    set(INSTALL_PATH_BIN ".")
    set(INSTALL_PATH_LIB "../lib")
    set(INSTALL_PATH_FRAMEWORKS "../Frameworks")
    set(INSTALL_PATH_PLUGIN "../plugins")
    set(INSTALL_PATH_DOC ".")
    set(INSTALL_PATH_APPLICATIONS ".")
    set(INSTALL_PATH_ICONS ".")
else()
    set(INSTALL_PATH_BIN "bin")
    set(INSTALL_PATH_SHARE "share/${APPLICATION_PACKAGE_NAME}")
    set(INSTALL_PATH_LIB "share/${APPLICATION_PACKAGE_NAME}/lib")
    set(INSTALL_PATH_PLUGIN "share/${APPLICATION_PACKAGE_NAME}/lib")
    set(INSTALL_PATH_DOC "share/${APPLICATION_PACKAGE_NAME}/doc")
    set(INSTALL_PATH_APPLICATIONS "share/applications")
    set(INSTALL_PATH_ICONS "share/icons")
endif()

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
set(QT4_COMPONENTS QtCore QtNetwork QtXml)
set(QT5_COMPONENTS Core Network Xml)

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
set(MOC_HEADERS
)
QT_WRAP_CPP(MOC_OUTFILES ${MOC_HEADERS})

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
