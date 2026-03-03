cmake_minimum_required(VERSION 3.23)
project(NetlistParser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(NetlistParser)
target_sources(NetlistParser
	PRIVATE
		main.cpp
)

add_subdirectory(dependencies)
target_link_libraries(NetlistParser
	PRIVATE	
		NetlistParserLib
)

