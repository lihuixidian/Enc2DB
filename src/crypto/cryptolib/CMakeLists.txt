cmake_minimum_required(VERSION 2.8)             #确定版本号，必须步骤

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE ON)

SET(TARGET udf)

SET(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS}   -maes -lgmp ") 

FILE(GLOB CFiles "libudf/*.c")
FILE(GLOB CPPFiles "libudf/*.cpp")
FILE(GLOB CPPFiles "libudf/decider/*.cpp")
set(FILES "${CFiles};${CPPFiles}")

# find_package(OpenSSL REQUIRED)



add_library(${TARGET} SHARED ${FILES})
include_directories(include libudf/decider/   ${OPENSSL_INCLUDE_DIRS} ${GMP_INCLUDE_DIRS}  symmetria )

target_link_libraries(${TARGET} ${OPENSSL_LIBRARIES})
target_link_libraries(${TARGET}  ${GMP_LIBRARIES})

