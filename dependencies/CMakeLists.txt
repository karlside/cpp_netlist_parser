add_library(NetlistParserLib)
target_sources(NetlistParserLib
	PRIVATE
		line.cpp
		netlist.cpp
		word.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES	
			containers.h
			exceptions.h
			keywords.h
			line.h
			netlist.h
			word.h
)
