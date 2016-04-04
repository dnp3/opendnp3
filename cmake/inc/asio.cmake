if (ASIO_HOME)
	message("ASIO_HOME defined in cache: ${ASIO_HOME}")
	include_directories(${ASIO_HOME})  
else()
	if(DEFINED ENV{ASIO_HOME})
		message("ASIO_HOME defined in environment: $ENV{ASIO_HOME}")  
		include_directories($ENV{ASIO_HOME})    
	else()
		message("ASIO_HOME was not defined. ASIO expected to be on include path")  
	endif()
endif()

# required for ASIO in C++11 only mode
add_definitions(-DASIO_STANDALONE)
