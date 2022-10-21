


set(Enclave_uh Enclave_u.h Enclave_u.c)

add_custom_command(OUTPUT ${Enclave_uh} PRE_BUILD
    COMMAND rm -f Enclave_u.*  && ${SGX_EDGER8R} --untrusted ${EDL_FILES_PATH}/Enclave.edl --search-path ${EDL_FILES_PATH} --search-path ${SGX_SDK}/include 
    COMMENT "make ucode again"
)

# add_executable(UDFTest UDFTest.cpp )
# target_link_libraries(UDFTest ${Udf_Lib} -Wl,--as-needed ${OPENSSL_LIBRARIES} ) 

add_executable(EnclaveTest EnclaveTest.cpp ${Enclave_uh})
target_link_libraries(EnclaveTest   -Wl,--as-needed ${Udf_Lib} -lsgx_uswitchless  ${jsoncpp_LIBRARY})