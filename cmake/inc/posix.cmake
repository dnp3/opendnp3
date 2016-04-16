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
	set(CMAKE_C_FLAGS "-g -O2 -Wall -W -Wno-multichar -Wunused-variable -Wno-unused-parameter -Wunused-function -Wunused -Wno-system-headers -Wwrite-strings -fprofile-arcs -ftest-coverage -save-temps")
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

	set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -Wno-parentheses --std=c++11")

	#find_library(ATOMIC_LIB atomic)
	#list(APPEND EXTRA_LIBS ${ATOMIC_LIB})

	# Compiler-specific C++11 activation.
	if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
		execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
		if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
			message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
		endif ()
	elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	else ()
		message(FATAL_ERROR "Your C++ compiler does not support C++11.")
	endif ()
	# different release and debug flags
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	if(COVERAGE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
		#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -g -O0")
	endif()

	if (WERROR)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif()

	# There's problem with detecting usage of pthread with gcc 4.8.x
	# http://ubuntuforums.org/showthread.php?t=2183408
	if(CMAKE_COMPILER_IS_GNUCXX)      
		if(CMAKE_CXX_COMPILER_VERSION MATCHES 4.8.*)
			message("Adding pthread workaround for GCC version: ${CMAKE_CXX_COMPILER_VERSION}")
			SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed" )            
		endif()
	endif()
endif()
