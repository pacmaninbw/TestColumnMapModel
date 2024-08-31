cmake_minimum_required(VERSION 3.5)

project(TestTableMap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestTableMap
    main.cpp
    columnaccessdata.cpp
    columnaccessdata.h
    tableidenum.h
    columnidenum.h
    hdb_columnnames.cpp
    hdb_columnnames.h
    hdb_columnnames.h
    dispalytodbtransferdata.h
    modelcolumntotablecolumnmap.h
    modelcolumntotablecolumnmap.cpp
    )

target_compile_features(TestTableMap PUBLIC cxx_std_20)
target_compile_definitions(TestTableMap PRIVATE MY_UNIT_TEST)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(TestTableMap PRIVATE /W4 /WX -D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # require at least gcc 12 to get full C++20 support
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12)
            message(FATAL_ERROR "GCC version must be at least 12!")
        endif()
    endif()
    # lots of warnings and all warnings as errors
    target_compile_options(TestTableMap PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
