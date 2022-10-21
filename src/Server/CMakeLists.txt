
set(target server)


if(${SGX_MODE} )
    set(Urts_Library_Name sgx_urts)
    # set(UAE_Server_Name sgx_uae_service)
ELSE()
    set(Urts_Library_Name sgx_urts_sim)
    # set(UAE_Server_Name sgx_uae_service_sim)
endif()


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_C_FLAGS " -maes  -std=gnu99 -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PB_DIR "${APP_DIR}/../protobuf")

if(${USE_SGX})
set(RAPB ../protobuf/RAMessages.pb.cc)
set(SGX_LIBRARY  -L${SGX_LIBRARY_PATH} -l${Urts_Library_Name} -L. -lsgx_ukey_exchange)
set(EDL_FILES_PATH ${APP_DIR}/Enclave/)
set(UNTRUSTED_BRIDGE ${APP_DIR}/Enclave_u.c ${APP_DIR}/Enclave_u.h)
add_custom_command(OUTPUT ${UNTRUSTED_BRIDGE} PRE_BUILD
    COMMAND cd ${APP_DIR}/ && rm -f Enclave_u.*  && ${SGX_EDGER8R} --untrusted ${EDL_FILES_PATH}/Enclave.edl --search-path ${EDL_FILES_PATH} --search-path ${SGX_SDK}/include 
    COMMENT "make ucode"
)

add_custom_command(OUTPUT ${PB_DIR}/RAMessages.pb.cc  PRE_BUILD
    COMMAND cd ${PB_DIR} && rm -f RAMessages.pb.cc  RAMessages.pb.h && protoc --cpp_out ./ RAMessages.proto
    COMMENT "make ra messages pb"
)
endif()

set(App_Cpp_Files "../utils/base64.c;../utils/config.cpp;App/Server.cpp;../utils/LogBase.cpp;../Networking/NetworkManager.cpp;../Networking/Session.cpp;../Networking/Server.cpp;\
../Networking/NetworkManagerServer.cpp;${RAPB};../Networking/AbstractNetworkOps.cpp;\
../utils/EnclaveManager.cpp;../MessageHandler/MessageHandler.cpp;../utils/UtilityFunctions.cpp")


include_directories( ../utils ${Protobuf_INCLUDE_DIR}  ${SGX_SDK}/include  ../Networking   ../protobuf  /usr/local/include ../MessageHandler)  #包含所需要的头文件，后面记得链接,相当于Makefile文件中“-L”选项
set(App_C_Flags " -Wno-attributescmake")#设置编译App文件夹中C文件的文件选项

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${App_Cpp_Flags}")         #将CMake中编译CPP文件中的编译选项设置成我们自己的选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${App_C_Flags}")            #将将CMake中编译C文件中的编译选项设置成我们自己的选项
# set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lcrypto  -lpthread  -L${SGX_LIBRARY_PATH} -l${Urts_Library_Name}")   
set(CMAKE_CXX_STANDARD 17)   #设置C++的std，相当于makefile文件中的-std=c++11

set(CURDIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${target} ${App_Cpp_Files})

target_link_libraries(${target} ${SGX_LIBRARY} -lpthread  -Wl,-rpath=${CURDIR} ${LOG4CPP_LIBRARIES}  ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${Protobuf_LIBRARY} jsoncpp)

