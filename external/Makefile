# Twese two usually works
HOSTARCH := arm
HOSTPARM := --host=arm-linux
# HOSTPARM := --host=arm-linux-androideabi
# PREFIXT="mipsel-linux-android"
# PREFIXT="i686-linux-android"
PREFIXT?="arm-linux-androideabi"
# edit line below if you have NDK to reflect the path where NDK is installed
NDK := $(NDK_BASE)
NDKDEST=/tmp/ndk-$(HOSTARCH)
NDKPLATFORM?=android-19
BUILDSYSTEM=linux-x86_64
NDKBOOST=/home/amoghbl1/apps/android_ndk/android-ndk-r10/
LDFLAGS ="-llog"

all: | maketoolchain build export
	@echo Built Successfully 

maketoolchain:
	bash $(NDKBOOST)/build/tools/make-standalone-toolchain.sh --platform=$(NDKPLATFORM) --install-dir=$(NDKDEST) --arch=$(HOSTARCH) --system=$(BUILDSYSTEM)

build: | buildboost moveboost configure compile
	@echo "Type 'build' successfuly built"

buildboost:
	cd Boost-for-Android && ./build-android.sh $(NDKBOOST) 

moveboost:
	rm -rf includes/curl
	cd Boost-for-Android && cp -r build/include/boost-1_53/boost/ ../includes/boost/

buildcurl:
	cd curl && \
	PATH=$(NDKDEST)/bin:$(PATH) ./configure --host=arm-linux-androideabi --disable-shared  && \
	PATH=$(NDKDEST)/bin:$(PATH) make

movecurl:
	rm -rf includes/curl
	cd curl && cp -r include/curl/ ../includes/
	cd includes/curl && sed -i "s/define CURL_SIZEOF_LONG           8/define CURL_SIZEOF_LONG           4/g" curlbuild.h.dist
	@echo "Fixing sizeof problem"

#configure:
#	cd opm && PATH=$(NDKDEST)/bin:$(PATH) LUA_CFLAGS="-DLUA_USE_POSIX" LDFLAGS="-static" \
#	ac_cv_linux_vers=2 CC=$(PREFIXT)-gcc CXX=$(PREFIXT)-g++ LD=$(PREFIXT)-ld RANLIB=$(PREFIXT)-ranlib AR=$(PREFIXT)-ar STRIP=$(PREFIXT)-strip \
#	./configure $(HOSTPARM) --without-zenmap --with-liblua=included --with-libpcap=internal --with-pcap=linux --enable-static --prefix=$(ANDROIDDEST) $(OPENSSLOPT)

compile:
	cd opm && \
	PATH=$(NDKDEST)/bin:$(PATH) LDFLAGS=" $(LDFLAGS) -I../includes/libcurl.a" $(PREFIXT)-g++ --sysroot=$(NDKDEST)/sysroot -I../includes/ -L../includes/ -pie -fPIE -fpermissive -std=c++11 -o ../bins/opm update.cpp download.cpp install.cpp list.cpp opm.cpp search.cpp -lcurl -lz && \
	PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-strip ../bins/opm

test:
	cd opm && \
    PATH=$(NDKDEST)/bin:$(PATH) LDFLAGS=" $(LDFLAGS) -I../includes/libcurl.a" $(PREFIXT)-g++ --sysroot=$(NDKDEST)/sysroot -I../includes/ -L../includes/ -pie -fPIE -fpermissive -std=c++11 -o test test.cpp -lcurl -lz && \
    PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-strip test

clean:
	rm -rf $(NDKDEST)

export:
	cp bins/* ../android_project/OpenPackageManager/app/src/main/res/raw

showsetup:
	@echo "Current environment"
