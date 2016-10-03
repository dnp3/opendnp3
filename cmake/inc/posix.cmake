if (UNIX)
	set(PTHREAD pthread)   
	find_package(Threads)
		if(STATICLIBS)
		set(LIB_TYPE STATIC)
	else()
		set(LIB_TYPE SHARED)
	endif()

	
	include (CheckLibraryExists)
	check_library_exists(pthread pthread_timedjoin_np "" HAVE_PHTREAD_TIMEDJOIN_NP)
	set(CMAKE_C_FLAGS "-g -O2 -Wall -W -Wno-multichar -Wunused-variable -Wno-unused-parameter -Wunused-function -Wunused -Wno-system-headers -Wwrite-strings -save-temps")
	if (${CYGWIN})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	elseif(${MINGW})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	else()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated")
 
	set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -Wno-parentheses --std=c++14")

	#find_library(ATOMIC_LIB atomic)
	#list(APPEND EXTRA_LIBS ${ATOMIC_LIB})

	# Compiler-specific C++14 activation.
	if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
		execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
		if (NOT (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9))
			message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.9 or greater.")
		endif ()
	elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	else ()
		message(FATAL_ERROR "Your C++ compiler does not support C++11.")
	endif ()

	if(COVERAGE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
	endif()

	if (WERROR)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif()
	
endif()
