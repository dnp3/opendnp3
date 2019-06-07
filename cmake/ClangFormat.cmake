find_program(CLANG_FORMAT_EXE
    NAMES clang-format
)
set_property(GLOBAL PROPERTY CLANG_FORMAT_FILES)

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
else()
    message(STATUS "clang-format not found")
endif()

function(clang_format target)

    set(options)
    set(oneValueArgs)
    set(multiValueArgs EXCLUDES)
    cmake_parse_arguments(clang_format "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(TARGET ${target})
        get_target_property(source_files ${target} SOURCES)
        get_property(all_source_files GLOBAL PROPERTY CLANG_FORMAT_FILES)

        foreach(file ${source_files})

            set(excluded FALSE)

            foreach(exclude ${clang_format_EXCLUDES})
                if(${file} MATCHES ${exclude})
                    set(excluded TRUE)
                endif()
            endforeach()

            if(NOT ${excluded})
                get_filename_component(relative_file ${file} ABSOLUTE)
                list(APPEND all_source_files ${relative_file})
            endif()

        endforeach()

        list(REMOVE_DUPLICATES all_source_files)
        set_property(GLOBAL PROPERTY CLANG_FORMAT_FILES ${all_source_files})
    else()
        message(WARNING "Target ${target} does not exists.")
    endif()

endfunction()


function(define_clang_format)
    if(CLANG_FORMAT_EXE)
        get_property(all_files GLOBAL PROPERTY CLANG_FORMAT_FILES)

        set(commands)
        foreach(file ${all_files})
            list(APPEND commands COMMAND ${CLANG_FORMAT_EXE} -i -style=file -fallback-style=none -verbose ${file})
        endforeach()

        list(LENGTH all_files num_files)
        add_custom_target(clang-format
            ${commands}
            COMMENT "Format ${num_files} source files"
        )
    endif()
endfunction()
