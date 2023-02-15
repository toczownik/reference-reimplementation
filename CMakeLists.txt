cmake_minimum_required(VERSION 3.16)
project(reference_reimplementation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE_DEBUG)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_finds")

set(PROGRAM_NAME reference_reimplementation)
add_executable(${PROGRAM_NAME} main.cpp Image_displayer.cpp Image_displayer.h)

option(OPENCV_SUPPORT_ENABLE "Enable image displaying using OpenCV library" ON)
option(CIMG_SUPPORT_ENABLE "Enable image displaying using OpenCV library" ON)

if (OPENCV_SUPPORT_ENABLE)
    find_package(OpenCV COMPONENTS core imgproc imgcodecs highgui)
endif()

if (CIMG_SUPPORT_ENABLE)
    find_package(Threads)
    find_package(X11)
endif()

find_package(GenICam REQUIRED)

if(CIMG_SUPPORT_ENABLE AND Threads_FOUND AND X11_FOUND)
    target_include_directories(${PROGRAM_NAME} PRIVATE  ${GENICAM_INCLUDE_DIR}
            ${X11_INCLUDE_DIR})
endif()

add_definitions(-DPCO_SUPPORT)

if (OPENCV_SUPPORT_ENABLE AND OpenCV_FOUND)
    add_definitions(-DOPENCV_SUPPORT)
    target_link_libraries(${PROGRAM_NAME}
            PRIVATE
            opencv_imgproc
            opencv_highgui
            opencv_imgcodecs
            opencv_core
            )
endif()

if (CIMG_SUPPORT_ENABLE AND Threads_FOUND AND X11_FOUND)
    add_definitions(-DCIMG_SUPPORT)
    target_link_libraries(${PROGRAM_NAME}
            PRIVATE
            ${X11_LIBRARIES}
            Threads::Threads
            )
endif()

target_link_libraries(${PROGRAM_NAME}
        PRIVATE
        -Wl,--no-as-needed
        genicam-mtca-ias::wrapmemcpy
        -Wl,--as-needed
        )

target_link_options(${PROGRAM_NAME} PRIVATE "LINKER:-wrap,memcpy")

find_package(GenicamWrapper REQUIRED)

include_directories(${GENICAM_WRAPPER_INCLUDE_DIR})
include_directories(${GENICAM_WRAPER_LIBRARY_DIR})
target_link_libraries(${PROGRAM_NAME} PRIVATE ${GENICAM_WRAPPER_LIBRARY_DIR})

find_package(genicam-mtca-ias 3.3.0 CONFIG REQUIRED)

target_link_libraries(${PROGRAM_NAME}
        PRIVATE
        ${GENAPI_LIBRARY}
        ${GCBASE_LIBRARY})

target_include_directories(${PROGRAM_NAME} PRIVATE ${GENICAM_INCLUDE_DIR})