target_sources(NetlistParserLib
	PRIVATE
		parsing_objects.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/parsing_objects.h
)
