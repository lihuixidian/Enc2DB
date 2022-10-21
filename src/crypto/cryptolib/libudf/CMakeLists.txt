# App CmakeLists

if(${USE_SGX} )

    if(${SGX_MODE} )
        set(Urts_Library_Name sgx_urts)
        # set(UAE_Server_Name sgx_uae_service)
    ELSE()
        set(Urts_Library_Name sgx_urts_sim)
        # set(UAE_Server_Name sgx_uae_service_sim)
    endif()
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_C_FLAGS " -maes  -std=gnu99 -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(${USE_SGX} )
    set(EDL_FILES_PATH ${APP_DIR}/Enclave/edls)
    set(UNTRUSTED_BRIDGE ${APP_DIR}/Enclave_u.c ${APP_DIR}/Enclave_u.h)
    add_custom_command(OUTPUT ${UNTRUSTED_BRIDGE} PRE_BUILD
        COMMAND cd ${APP_DIR}/ && rm -f Enclave_u.*  && ${SGX_EDGER8R} --untrusted ${EDL_FILES_PATH}/Enclave.edl --search-path ${EDL_FILES_PATH} --search-path ${SGX_SDK}/include 
        COMMENT "make ucode"
    )
endif()



FILE(GLOB App_C_Files "./*.c")
FILE(GLOB App_Cpp_Files "./*.cpp")
FILE(GLOB CPPFiles "decider/*.cpp")
set(App_Cpp_Files "${App_Cpp_Files};${CPPFiles};../../../utils/EnclaveManager.cpp;../../../utils/config.cpp;../../../utils/TimeCounter.cpp")

# file(GLOB_RECURSE App_Cpp_Files "*.cpp")   #将App文件夹中的cpp文件列表赋值给APP_CPP_FILES
# file(GLOB_RECURSE App_C_Files "*.c")   #将App文件夹中的cpp文件列表赋值给APP_CPP_FILES

include_directories(${PROJECT_SOURCE_DIR}/include ${APP_DIR} ${SGX_SDK}/include)  #包含所需要的头文件，后面记得链接,相当于Makefile文件中“-L”选项
set(App_C_Flags " -Wno-attributescmake")#设置编译App文件夹中C文件的文件选项

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${App_Cpp_Flags}")         #将CMake中编译CPP文件中的编译选项设置成我们自己的选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${App_C_Flags}")            #将将CMake中编译C文件中的编译选项设置成我们自己的选项
# set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lcrypto  -lpthread  -L${SGX_LIBRARY_PATH} -l${Urts_Library_Name}")   
set(CMAKE_CXX_STANDARD 17)   #设置C++的std，相当于makefile文件中的-std=c++11

#  target_include_directories(${SGX_LIBRARY_PATH})
if(${SGX_MODE} ) # for sim mode
    # set(uRTS_PATH "/usr/lib/x86_64-linux-gnu")
endif()


add_library(${Udf_Lib} SHARED ${App_Cpp_Files} ${App_C_Files} ${UNTRUSTED_BRIDGE})

if(${USE_SGX}) 
    target_link_libraries(${Udf_Lib} -L${SGX_LIBRARY_PATH} -Wl,--whole-archive -lsgx_uswitchless -lsgx_uprotected_fs -Wl,--no-whole-archive   -l${Urts_Library_Name}) 
endif()

target_link_libraries(${Udf_Lib} ${SYM_Lib} ${OPENSSL_LIBRARIES}  -lpthread  ${jsoncpp_LIBRARY})

if(${USE_SGX}) 
    add_subdirectory(Enclave)
    add_subdirectory(tests)
endif()

