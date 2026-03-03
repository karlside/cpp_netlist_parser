target_sources(NetlistParserLib
	PRIVATE
		parsing_objects.cpp
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		FILES
			parsing_objects.h
)
