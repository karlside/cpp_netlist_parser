add_library(NetlistParserLib)

target_include_directories(NetlistParserLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}           
)

target_sources(NetlistParserLib
	PRIVATE
		netlist.cpp
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		FILES
			netlist.h
)

add_subdirectory(input_parser)
add_subdirectory(netlist_objects)
add_subdirectory(misc)


