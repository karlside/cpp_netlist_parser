target_sources(NetlistParserLib
	PRIVATE
		word_parser.cpp
		line_parser.cpp
		netlist_parser.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/word_parser.h
			${CMAKE_CURRENT_SOURCE_DIR}/line_parser.h
			${CMAKE_CURRENT_SOURCE_DIR}/netlist_parser.h
)
