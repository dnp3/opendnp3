find_program(CLANG_TIDY_EXE
    NAMES clang-tidy
)
set_property(GLOBAL PROPERTY CLANG_TIDY_FILES)

if(CLANG_TIDY_EXE)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Enable clang-tidy integration")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*")
else()
    message(STATUS "clang-tidy not found")
endif()

function(clang_tidy target)
    if(TARGET ${target})
        if(CLANG_TIDY_EXE)
            # Enable CMake integration of clang-tidy when compiling
            if(ENABLE_CLANG_TIDY)
                set_target_properties(${target} PROPERTIES
                    C_CLANG_TIDY "${DO_CLANG_TIDY}"
                    CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
            endif()
            
            # Add the source file to the global property
            get_target_property(source_files ${target} SOURCES)
            get_property(all_tidy_files GLOBAL PROPERTY CLANG_TIDY_FILES)
            foreach(file ${source_files})
                if(NOT ${file} MATCHES "h$")
                    list(APPEND all_tidy_files ${file})
                endif()
            endforeach()
            list(REMOVE_DUPLICATES all_tidy_files)
            set_property(GLOBAL PROPERTY CLANG_TIDY_FILES ${all_tidy_files})
        endif()
    else()
        message(WARNING "Target ${target} does not exists.")
    endif()
endfunction()

function(define_clang_tidy)
    if(CLANG_TIDY_EXE)
        get_property(all_tidy_files GLOBAL PROPERTY CLANG_TIDY_FILES)

        set(commands)
        foreach(file ${all_tidy_files})
            list(APPEND commands COMMAND ${CLANG_TIDY_EXE} -fix -p ${CMAKE_BINARY_DIR} ${file})
        endforeach()

        message(STATUS ${CMAKE_BINARY_DIR})
        list(LENGTH all_tidy_files num_files)
        add_custom_target(clang-tidy
            ${commands}
            COMMENT "Applying clang-tidy fixes on ${num_files} files"
        )
    endif()
endfunction()
