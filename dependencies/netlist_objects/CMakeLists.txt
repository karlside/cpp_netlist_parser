target_sources(NetlistParserLib
	PRIVATE
		word.cpp
		line.cpp
		netlist.cpp
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		FILES
			word.h
			line.h
			netlist.h
)
