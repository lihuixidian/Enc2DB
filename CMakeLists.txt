cmake_minimum_required(VERSION 3.0)             #确定版本号，必须步骤
project(encryptsql)                           #项目名称，随便写
set(TARGET encryptsql)
set(top_builddir ../../..)
######################## common settings #############################################
option(USE_ENCRYPT "Whether to use encrypt function" OFF)
option(TIME_PORTRAIT "Whether to use time portrait" OFF)

option(USE_LRU "Whether to use LRU" OFF)
option(USE_SGX "Whether to use SGX" OFF)
option(USE_SWITCHLESS "Whether to use SGX switchless" OFF)
option(SGX_MODE "on for hw mode, off for sim mode" ON)

option(SGX_ORE "Whether to use SGX for all ORE calculation" OFF)   
option(SGX_MHE "Whether to use SGX for all MHE calculation" OFF)

set(SGX_SDK /sgx/release/sgxsdk)                  #SDK的安装路径
set(SGX_ARCH x64)
# set(SGX_DEBUG 1)
# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")
#####################################################################################

set(SGX_COMMON_FLAGS -m64)
set(SGX_LIBRARY_PATH ${SGX_SDK}/lib64)
set(SGX_EDGER8R ${SGX_SDK}/bin/x64/sgx_edger8r)
set(SGX_ENCLAVE_SIGNER ${SGX_SDK}/bin/x64/sgx_sign) #以上的参数都是根据自己的机器设置，注意不要设置错误

if(SGX_ORE OR SGX_MHE)
    if(NOT USE_SGX)
        message(FATAL_ERROR "You want to use SGX to calculate ORE or MHE, but you didn't turn on the switch of SGX！")
    endif()
endif()


set(CMAKE_VERBOSE_MAKEFILE ON)

if(${TIME_PORTRAIT})
    add_definitions(-DTIME_PORTRAIT)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DEDEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

if(${USE_LRU} )
    add_definitions(-DUSE_LRU)
endif()

if(${USE_SWITCHLESS} )
    add_definitions(-DUSE_SWITCHLESS)
endif()

if(${USE_ENCRYPT})
    add_definitions(-DUSE_ENCRYPT)
endif()

if(USE_SGX)
    add_definitions(-DUSE_SGX)
    message(STATUS "Build with SGX!")

    if(${SGX_ORE})
    add_definitions(-DSGX_ORE)
    endif() 

    if(${SGX_MHE})
        add_definitions(-DSGX_MHE)
    endif()
else()
    message(STATUS "Build without SGX!")
endif()


# SET(CMAKE_C_FLAGS "-fno-omit-frame-pointer -fsanitize=address -maes  -std=gnu99 -fvisibility=hidden -Wall -Wunused  -Wmissing-prototypes -Wpointer-arith -Wendif-labels -Wmissing-format-attribute -Wformat-security -fno-strict-aliasing -fwrapv -fexcess-precision=standard") 
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++11 -fno-omit-frame-pointer -fsanitize=address") 

SET(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -maes  -std=gnu99 -fvisibility=hidden -Wall -Wunused  -Wmissing-prototypes -Wpointer-arith -Wendif-labels -Wmissing-format-attribute -Wformat-security -fno-strict-aliasing -fwrapv -fexcess-precision=standard") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++11  ") 


SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}" )

message(${CMAKE_MODULE_PATH})
file(GLOB_RECURSE Cpp_Files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  "src/encryptsql/*.cpp" )  
file(GLOB_RECURSE C_Files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/encryptsql/*.c")  
# file(GLOB_RECURSE UtilsFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/utils/*.cpp")  


set(Files "${Cpp_Files};${C_Files};src/utils/cryptoUtils.cpp;src/utils/cryptoCaller.cpp;src/utils/utils.cpp;src/utils/base64.c;src/utils/opensslUtils.cpp")


# Filter values through regex
#   filter_regex({INCLUDE | EXCLUDE} <regex> <listname> [items...])
#   Element will included into result list if
#     INCLUDE is specified and it matches with regex or
#     EXCLUDE is specified and it doesn't match with regex.
# Example:
#   filter_regex(INCLUDE "(a|c)" LISTOUT a b c d) => a c
#   filter_regex(EXCLUDE "(a|c)" LISTOUT a b c d) => b d
function(filter_regex _action _regex _listname)
    # check an action
    if("${_action}" STREQUAL "INCLUDE")
        set(has_include TRUE)
    elseif("${_action}" STREQUAL "EXCLUDE")
        set(has_include FALSE)
    else()
        message(FATAL_ERROR "Incorrect value for ACTION: ${_action}")
    endif()

    set(${_listname})
    foreach(element ${ARGN})
        string(REGEX MATCH ${_regex} result ${element})
        if(result)
            if(has_include)
                list(APPEND ${_listname} ${element})
            endif()
        else()
            if(NOT has_include)
                list(APPEND ${_listname} ${element})
            endif()
        endif()
    endforeach()

    # put result in parent scope variable
    set(${_listname} ${${_listname}} PARENT_SCOPE)
endfunction()


function(combine_archives output_archive list_of_input_archives)
    set(TEMP_DIR /tmp)
    set(mri_file ${TEMP_DIR}/${output_archive}.mri)
    set(FULL_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/lib${output_archive}.a)
    file(WRITE ${mri_file} "create ${FULL_OUTPUT_PATH}\n")
    FOREACH(in_archive ${list_of_input_archives})
        file(APPEND ${mri_file} "addlib ${LIBRARY_OUTPUT_PATH}/lib${in_archive}.a\n")
    ENDFOREACH()
    file(APPEND ${mri_file} "save\n")
    file(APPEND ${mri_file} "end\n")

    set(output_archive_dummy_file ${TEMP_DIR}/${output_archive}.dummy.cpp)
    add_custom_command(OUTPUT ${output_archive_dummy_file}
                       COMMAND touch ${output_archive_dummy_file}
                       DEPENDS ${list_of_input_archives})

    add_library(${output_archive} STATIC ${output_archive_dummy_file})
    add_custom_command(TARGET ${output_archive}
                       POST_BUILD
                       COMMAND ar -M < ${mri_file})
endfunction(combine_archives)


filter_regex(EXCLUDE "libudf" Files ${Files})
filter_regex(EXCLUDE "Interface_c" Files ${Files})
filter_regex(EXCLUDE "symmetria" Files ${Files})

 
# message(STATUS  ${SOURCES})

# find_package(PkgConfig REQUIRED)
# pkg_search_module(OPENSSL REQUIRED openssl)
# pkg_search_module(GMP REQUIRED gmp)
find_package(jsoncpp REQUIRED)
find_package(GMP REQUIRED)
find_package(LOG4CPP REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system thread REQUIRED)
find_package(Protobuf REQUIRED)

message("FOUND CONFIG: ${jsoncpp_CONFIG}")

get_target_property(jsoncpp_INCLUDE jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
set(jsoncpp_LIBRARY jsoncpp_lib)
set(jsoncpp_STATIC_LIBRARY jsoncpp_lib_static)
SET(CryptoLib src/crypto/cryptolib)

include_directories(${top_builddir}/include include src/utils ${OPENSSL_INCLUDE_DIR} ${GMP_INCLUDE_DIR}  ${CryptoLib}/symmetria ${jsoncpp_INCLUDE})

SET(Udf_Lib udf)
SET(SYM_Lib symmetria)
SET(HE_Lib he)
SET(ORE_Lib ore)
add_subdirectory(${CryptoLib})
# add_subdirectory(src/Server)

if(${CMAKE_BUILD_TYPE} EQUAL "Debug")
    add_subdirectory(tests)
endif()
add_subdirectory(tests)
add_library(${TARGET} SHARED ${Files})
target_link_libraries(${TARGET}  ${OPENSSL_LIBRARIES} ${jsoncpp_LIBRARY})
target_link_libraries(${TARGET}  ${SYM_Lib})

# if(${USE_SGX})
#     add_custom_target( genKey ALL
#                     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/EnclaveTest
#                     DEPENDS ${TARGET} EnclaveTest
#                     COMMENT "generate enclave key"
#     )
# endif()
