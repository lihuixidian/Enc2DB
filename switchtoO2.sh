#!/usr/bin/env bash
# Cur_Dir=$(pwd)


# echo "{}" > /etc/encryptsql/map.json

workdir=$(cd $(dirname $0); pwd)
top_dir=${workdir}/../../../..

if [ -z $1 ]; then echo "need parameter"; exit -1; fi

if [ $1 == 'clean' ]; then 
   echo "{}" > /etc/encryptsql/map.json
   echo "" > ~/sysbench-tpcc-master/logfile

fi



if [ $1 == 'ndebug' ]; then 
    cd ${top_dir} && ./configure --disable-debug && cd ${top_dir}/bin/psql && make clean -j && make -j 
    cd ${workdir} && sed -i 's/set(CMAKE_BUILD_TYPE "Debug")/set(CMAKE_BUILD_TYPE "Release")/g' CMakeLists.txt
    cd ${workdir}/.. &&  make clean && make -j && chmod 777 libpq.so.* && make install -j
fi


if [ 'debug' == $1 ]; then 
    echo ${top_dir}
    cd ${top_dir} && ./configure --enable-debug && cd ${top_dir}/bin/psql  && make clean -j  && make -j 
    cd ${workdir} && sed -i 's/set(CMAKE_BUILD_TYPE "Release")/set(CMAKE_BUILD_TYPE "Debug")/g' CMakeLists.txt
    cd ${workdir}/.. &&  make clean && make -j && chmod 777 libpq.so.*  && make install -j
fi

if [ 'crypt' == $1 ]; then 

    # cd ${workdir} && sed -i 's/set(CMAKE_BUILD_TYPE "Release")/set(CMAKE_BUILD_TYPE "Debug")/g' CMakeLists.txt
    cd ${workdir}/.. && sed -i 's/USE_TRANSFORM = 0/USE_TRANSFORM = 1/g' Makefile && sed -i 's/\/\/ disableAsNeeded();/disableAsNeeded();/g' fe-exec.c &&  \
         make clean && make -j && chmod 777 libpq.so.5.9  && make install -j
fi

if [ 'ncrypt' == $1 ]; then 

    # cd ${workdir} && sed -i 's/set(CMAKE_BUILD_TYPE "Release")/set(CMAKE_BUILD_TYPE "Debug")/g' CMakeLists.txt
    cd ${workdir}/.. && sed -i 's/USE_TRANSFORM = 1/USE_TRANSFORM = 0/g' Makefile && sed -i 's/disableAsNeeded();/\/\/ disableAsNeeded();/g' fe-exec.c &&  \
         make clean && make -j && chmod 777 libpq.so.5.9  && make install -j
fi