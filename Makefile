

subdir = src/interfaces/libpq/encryptsql
top_builddir = ../../../..

include $(top_builddir)/src/Makefile.global

# mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))

# mkfile_path := $(shell pwd) 
# export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:mkfile_path
# export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(mkfile_path)
# $(shell )
# NAME= encryptsql

# SO_MAJOR_VERSION= 0
# SO_MINOR_VERSION= 1

# include $(top_srcdir)/src/Makefile.shlib


build/lib/libencryptsql.so: 

	$(shell if [ ! -d build ]; then mkdir build; fi;)
	$(shell if [ ! -d build/lib ]; then mkdir build/lib; fi;)
	$(shell if [ ! -d /etc/encryptsql ]; then sudo mkdir -p /etc/encryptsql; fi;)
	$(shell touch build/lib/enclave.signed.so)
	# cd build && export CC=/opt/rh/devtoolset-9/root/usr/bin/gcc && export CXX=/opt/rh/devtoolset-9/root/usr/bin/g++ && cmake ../CMakeLists.encryptsql && make
	# cd build && cmake -DCMAKE_C_COMPILER=/opt/rh/devtoolset-9/root/usr/bin/gcc   -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-9/root/usr/bin/g++ .. && make
	# cd build && cmake -DCMAKE_C_COMPILER=$(C)  -DCMAKE_CXX_COMPILER=$(CC) .. && make -j8
	cd build && cmake -DCMAKE_C_COMPILER=/usr/bin/gcc  -DCMAKE_CXX_COMPILER=/usr/bin/g++ .. && make 

	# cd build && cmake .. && make 
	# sudo cp build/lib/libencryptsql.so /lib
	chmod 777 build/lib/enclave.signed.so
	chmod 777 build/lib/libudf.so
	cp build/lib/enclave.signed.so /etc/encryptsql/
	# build/bin/EnclaveTest

all: build/lib/libencryptsql.so

clean: clean-lib
	rm -rf build
	rm -rf `pg_config --libdir/libudf.so`
	rm -f src/crypto/cryptolib/libudf/Enclave_u.*
	rm -f src/crypto/cryptolib/libudf/Enclave/Enclave_t.*


include $(top_srcdir)/src/Makefile.shlib
backend_src = $(top_srcdir)/src/common/backend


install: all
	$(INSTALL_DATA) build/lib/libencryptsql.so '$(DESTDIR)$(libdir)'
	$(INSTALL_DATA) build/lib/libudf.so '$(DESTDIR)$(libdir)'


# ./config no-autoalginit no-autoerrinit no-comp no-ct  no-dso no-ec no-ec2m no-engine no-err no-gost  no-pic no-pinshared no-posix-io  no-psk no-rdrand  no-shared no-sock no-sse2 no-stdio CFLAGS="-nostdinc" CXXFLAGS="-nostdinc++" LDFLAGS="-Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles"

# ./config no-autoalginit no-autoerrinit no-comp no-ct  no-dso no-ec no-ec2m no-engine no-err no-gost  no-pic no-pinshared no-posix-io  no-psk no-rdrand  no-shared no-sock no-sse2 no-stdio 
# CFLAGS="-nostdinc" CXXFLAGS="-nostdinc++" LDFLAGS="-Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles" make