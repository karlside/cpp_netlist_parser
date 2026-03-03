target_sources(NetlistParserLib
	PRIVATE
		word.cpp
		line.cpp
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS 
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/word.h
			${CMAKE_CURRENT_SOURCE_DIR}/line.h
)
