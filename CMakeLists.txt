cmake_minimum_required(VERSION 3.16)
project(reference_reimplementation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE_DEBUG)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_finds")

set(PROGRAM_NAME reference_reimplementation)
add_executable(${PROGRAM_NAME} main.cpp)

find_package(GenICam REQUIRED)
find_package(GenicamWrapper REQUIRED)

include_directories(${GENICAM_WRAPPER_INCLUDE_DIR})
include_directories(${GENICAM_WRAPER_LIBRARY_DIR})
target_link_libraries(${PROGRAM_NAME} ${GENICAM_WRAPPER_LIBRARY_DIR})

find_package(genicam-mtca-ias 3.3.0 CONFIG REQUIRED)

target_link_libraries(${PROGRAM_NAME} ${CMAKE_DL_LIBS})
target_link_libraries(${PROGRAM_NAME}
        ${GENAPI_LIBRARY}
        ${GCBASE_LIBRARY})

link_libraries(${GENAPI_LIBRARY} ${GCBASE_LIBRARY})

target_include_directories(${PROGRAM_NAME} PRIVATE ${GENICAM_INCLUDE_DIR})