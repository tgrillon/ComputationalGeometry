function(AddCppCheck target)
    find_program(CPPCHECK_PATH cppcheck REQUIRED)
    
    # Get target properties
    get_target_property(TARGET_SOURCES ${target} SOURCES)
    get_target_property(SOURCE_DIR ${target} SOURCE_DIR)
    get_target_property(INCLUDE_DIRS ${target} INCLUDE_DIRECTORIES)
    
    # Build include arguments
    set(INCLUDE_ARGS "")
    if(INCLUDE_DIRS)
        foreach(inc_dir ${INCLUDE_DIRS})
            # Skip external include directories
            if(NOT inc_dir MATCHES "/_deps/|/external/|/third_party/")
                list(APPEND INCLUDE_ARGS "-I${inc_dir}")
            endif()
        endforeach()
    endif()
    
    # Filter sources to exclude external code and specific files
    set(SOURCES_TO_CHECK "")
    foreach(source ${TARGET_SOURCES})
        get_filename_component(ABS_SOURCE "${source}" ABSOLUTE BASE_DIR "${SOURCE_DIR}")
        get_filename_component(FILENAME "${source}" NAME)
        
        # Only include project sources (exclude external and stb_image)
        if(NOT ABS_SOURCE MATCHES "/_deps/|/external/|/third_party/|FetchContent")
            # Exclude stb_image files specifically
            if(NOT FILENAME MATCHES "stb_image\\.(cpp|h)")
                # Only check .cpp, .cc, .cxx files (not headers from external libs)
                if(source MATCHES "\\.(cpp|cc|cxx)$")
                    list(APPEND SOURCES_TO_CHECK ${ABS_SOURCE})
                endif()
            endif()
        endif()
    endforeach()
    
    if(SOURCES_TO_CHECK)
        # Create custom target for CppCheck
        add_custom_target(
            cppcheck-${target}
            COMMAND ${CPPCHECK_PATH}
                --enable=warning,style,performance,portability
                --error-exitcode=10
                --inline-suppr
                --suppress=*:*/_deps/*
                --suppress=*:*/external/*
                --suppress=*:*/third_party/*
                --suppress=*:*/stb_image.cpp
                --suppress=*:*/stb_image.h
                --std=c++23
                --language=c++
                ${INCLUDE_ARGS}
                ${SOURCES_TO_CHECK}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running CppCheck on ${target}"
            VERBATIM
        )
        
        # Add to main cppcheck target if it exists, or create it
        if(NOT TARGET cppcheck)
            add_custom_target(cppcheck)
        endif()
        add_dependencies(cppcheck cppcheck-${target})
    else()
        message(STATUS "No sources to check for target ${target}")
    endif()
endfunction()
