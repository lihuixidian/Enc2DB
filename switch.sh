#!/usr/bin/env bash
# Cur_Dir=$(pwd)


# echo "{}" > /etc/encryptsql/map.json

workdir=$(cd $(dirname $0); pwd)
top_dir=${workdir}/../../../..

if [ -z $1 ]; then echo "need parameter"; exit -1; fi

if [ $1 == 'clean' ]; then 
   echo "{}" > /etc/encryptsql/map.json
#    echo "" > ~/sysbench-tpcc-master/logfile

fi



if [ $1 == 'ndebug' ]; then 
    cd ${top_dir} && ./configure --disable-debug 
    cd ${workdir} && sed -i 's/set(CMAKE_BUILD_TYPE "Debug")/set(CMAKE_BUILD_TYPE "Release")/g' CMakeLists.txt
    # cd ${workdir}/.. &&  make clean && make -j && chmod 777 libpq.so.* && make install -j
fi


if [ 'debug' == $1 ]; then 

    sed -i 's/set(CMAKE_BUILD_TYPE "Release")/set(CMAKE_BUILD_TYPE "Debug")/g' CMakeLists.txt
    make clean && make -j && make install 
fi

if [ 'crypt' == $1 ]; then 

    sed -i 's/option(USE_ENCRYPT "Whether to use encrypt function" OFF)/option(USE_ENCRYPT "Whether to use encrypt function" ON)/g' CMakeLists.txt
    make clean && make -j && make install 
fi

if [ 'ncrypt' == $1 ]; then 

    sed -i 's/option(USE_ENCRYPT "Whether to use encrypt function" ON)/option(USE_ENCRYPT "Whether to use encrypt function" OFF)/g' CMakeLists.txt
    make clean && make -j && make install 
fi

if [ 'ore' == $1 ]; then 
    cp ~/pgdata_bak/map.json.sgxore /etc/encryptsql/map.json 
    sed -i 's/option(USE_SGX "Whether to use SGX" OFF)/option(USE_SGX "Whether to use SGX" ON)/g' CMakeLists.txt
    make clean && make -j && make install 
    ./build/bin/EnclaveTest
fi

if [ 'soft' == $1 ]; then 
    cp ~/pgdata_bak/map.json.soft /etc/encryptsql/map.json 
    sed -i 's/option(USE_SGX "Whether to use SGX" ON)/option(USE_SGX "Whether to use SGX" OFF)/g' CMakeLists.txt
    make clean && make -j && make install 
fi