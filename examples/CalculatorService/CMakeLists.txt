cmake_minimum_required(VERSION 3.1.0)

project(CalculatorService VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt5 COMPONENTS Core Xml Network REQUIRED)

# Add service
add_subdirectory (calculator)
include_directories(calculator)

target_include_directories (calculator PRIVATE ${Qt5Core_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})

# Create executable
add_executable(CalculatorService
	main.cpp
)

target_link_libraries (CalculatorService calculator Qt5::Core Qt5::Xml Qt5::Network)
