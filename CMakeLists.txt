cmake_minimum_required(VERSION 3.5)

project(TestTableMap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestTableMap
    main.cpp
    columnaccessdata.cpp
    columnaccessdata.h
    columnidenum.h
    hdb_columnnames.cpp
    hdb_columnnames.h
    hdb_columnnames.h
    dispalytodbtransferdata.h
    modelcolumntotablecolumnmap.h
    modelcolumntotablecolumnmap.cpp
    )
