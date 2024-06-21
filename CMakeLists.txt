cmake_minimum_required (VERSION 2.8)

project (safe-evpp C CXX)

option(EVPP__BUILD_32_BIT "Build evpp and dependent apps as 32-bit libs" OFF)
option(EVPP__USE_CXX11_ABI "Build evpp with C++11 compatible ABI" ON)

if (UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    set (CXX_FLAGS
         -g
         -fPIC
         -Wall
         -Wno-unused-parameter
         -Wno-unused-function
         -Wunused-variable
         -Wunused-value
         -Wshadow
         -Wcast-qual
         -Wcast-align
         -Wwrite-strings
         -Wsign-compare
         -Winvalid-pch
         -fms-extensions
         -Wfloat-equal
         -Wextra
         -std=c++11
         -Wl,-rpath '-Wl,\$ORIGIN'
    )

    if(CMAKE_INCLUDE_PATH)
        set(INCLUDE_DIRS ${CMAKE_INCLUDE_PATH})
    endif()
    list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty)
    include_directories(${INCLUDE_DIRS})

    if(CMAKE_LIBRARY_PATH)
        link_directories(${CMAKE_LIBRARY_PATH})
    endif()

    if (EVPP__BUILD_32_BIT)
        list(APPEND CXX_FLAGS -m32)
    endif(EVPP__BUILD_32_BIT)

    if (EVPP__USE_CXX11_ABI)
        list(APPEND CXX_FLAGS -D_GLIBCXX_USE_CXX11_ABI=1)
    else (EVPP__USE_CXX11_ABI)
        list(APPEND CXX_FLAGS -D_GLIBCXX_USE_CXX11_ABI=0)
    endif(EVPP__USE_CXX11_ABI)

else (UNIX)
set (CXX_FLAGS -DWIN32 -D_WIN32 -EHsc)
    if(CMAKE_INCLUDE_PATH)
        set(INCLUDE_DIRS ${CMAKE_INCLUDE_PATH})
        list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty/wininclude)
        include_directories(${INCLUDE_DIRS})
    else()
        set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
        include_directories(${VcpkgRoot}/include
                            ${PROJECT_SOURCE_DIR}
                            ${PROJECT_SOURCE_DIR}/3rdparty/wininclude)
    endif()

    if(CMAKE_LIBRARY_PATH)
        link_directories(${CMAKE_LIBRARY_PATH})
    else()
        # On windows, Debug mode
        link_directories(${VcpkgRoot}/debug/lib ${VcpkgRoot}/debug/lib/manual-link)

        # On Windows, Release mode
        # link_directories(${VcpkgRoot}/lib ${VcpkgRoot}/lib/manual-link)
    endif()
endif (UNIX)

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if (UNIX)
    SET(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb -D_DEBUG -DGOOGLE_STRIP_LOG=0")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -ggdb -DNDEBUG -DGOOGLE_STRIP_LOG=1")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        SET(DEPENDENT_LIBRARIES event_core glog pthread)
    else()
        SET(DEPENDENT_LIBRARIES event_core glogd pthread)
    endif()
else (UNIX)
    SET(DEPENDENT_LIBRARIES event glog)
endif (UNIX)

# Set to true if https support is needed.
# Note that this needs openssl
# SET(HTTPS True)
if (HTTPS)
    list(APPEND DEPENDENT_LIBRARIES ssl crypto event_openssl)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_CLIENT_SUPPORTS_SSL")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_SERVER_SUPPORTS_SSL")
endif (HTTPS)


if (CMAKE_BENCHMARK_TESTING)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DH_BENCHMARK_TESTING=1")
endif (CMAKE_BENCHMARK_TESTING)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set (MACOSX_RPATH ${LIBRARY_OUTPUT_PATH})
set (CMAKE_MACOSX_RPATH 0)

find_program(THRIFT_COMPILER thrift)
find_path(THRIFT_INCLUDE_DIR thrift)
find_library(THRIFT_LIBRARY NAMES thrift)

message (STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message (STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message (STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message (STATUS "CMAKE_INCLUDE_PATH " ${CMAKE_INCLUDE_PATH})
message (STATUS "CMAKE_LIBRARY_PATH " ${CMAKE_LIBRARY_PATH})
message (STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message (STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})
message (STATUS "THRIFT_COMPILER " ${THRIFT_COMPILER})
message (STATUS "THRIFT_INCLUDE_DIR " ${THRIFT_INCLUDE_DIR})
message (STATUS "THRIFT_LIBRARY " ${THRIFT_LIBRARY})

add_subdirectory (evpp)
add_subdirectory (apps)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include (utils)
include (packages)

include (CPack)
include (CTest)
