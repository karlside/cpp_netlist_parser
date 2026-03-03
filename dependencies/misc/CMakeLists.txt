target_sources(NetlistParserLib
	PUBLIC
		FILE_SET HEADERS
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/containers.h
			${CMAKE_CURRENT_SOURCE_DIR}/exceptions.h
			${CMAKE_CURRENT_SOURCE_DIR}/keywords.h
)
