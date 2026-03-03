target_sources(NetlistParserLib
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		FILES
			containers.h
			exceptions.h
			keywords.h
)
