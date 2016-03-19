# Twese two usually works
HOSTARCH := arm
HOSTPARM := --host=arm-linux
# HOSTPARM := --host=arm-linux-androideabi
# PREFIXT="mipsel-linux-android"
# PREFIXT="i686-linux-android"
PREFIXT?="arm-linux-androideabi"
# edit line below if you have NDK to reflect the path where NDK is installed
NDK := $(NDK_BASE)
ANDROID_NDK_ROOT := $(NDK_BASE)
NDKDEST=/tmp/ndk-$(HOSTARCH)
NDKPLATFORM?=android-19
BUILDSYSTEM=linux-x86_64
NDKBOOST=/home/amoghbl1/apps/android_ndk/android-ndk-r10/
MAKEFILE_LOCATION=/home/amoghbl1/git/AndroidPackageManager/external
LDFLAGS ="-llog"

all: maketoolchain build export
	@echo Built Successfully 

maketoolchain:
	bash $(NDKBOOST)/build/tools/make-standalone-toolchain.sh --platform=$(NDKPLATFORM) --install-dir=$(NDKDEST) --arch=$(HOSTARCH) --system=$(BUILDSYSTEM)

build: buildboost moveboost buildcurl movecurl compile
	@echo "Type 'build' successfuly built"

buildboost:
	cd Boost-for-Android && ./build-android.sh $(NDKBOOST) 

moveboost:
	rm -rf includes/curl
	cd Boost-for-Android && cp -r build/include/boost-1_53/boost/ ../includes/boost/

buildcurl: buildopenssl
	cd curl && \
	PATH=$(NDKDEST)/bin:$(PATH) \
		./configure \
			--host=arm-linux-androideabi && \
	PATH=$(NDKDEST)/bin:$(PATH) make


buildopenssl:
	cd openssl && \
	./setenv-android.sh && \
	./config shared no-ssl3 no-comp no-hw no-engine --openssldir=/usr/local/ssl/android-14/ && \
	make depend && \
	make all

movecurl:
	rm -rf includes/curl
	cd curl && cp -r include/curl/ ../includes/ && cp lib/.libs/libcurl.a ../includes/
	cd includes/curl && sed -i "s/define CURL_SIZEOF_LONG           8/define CURL_SIZEOF_LONG           4/g" *
	@echo "Fixing sizeof problem"

compile:
	cd opm && \
	PATH=$(NDKDEST)/bin:$(PATH) \
	LDFLAGS=" $(LDFLAGS) -I../includes/libcurl.a" \
	$(PREFIXT)-g++ \
		--sysroot=$(NDKDEST)/sysroot \
		-I../includes/ \
		-L../includes/ \
		-pie -fPIE \
		-fpermissive \
		-std=c++11 \
		-o ../bins/opm \
		update.cpp download.cpp install.cpp list.cpp opm.cpp search.cpp remove.cpp\
		-lcurl -lz && \
	PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-strip ../bins/opm

test:
	cd opm && \
    PATH=$(NDKDEST)/bin:$(PATH) LDFLAGS=" $(LDFLAGS) -I../includes/libcurl.a" $(PREFIXT)-g++ --sysroot=$(NDKDEST)/sysroot -I../includes/ -L../includes/ -pie -fPIE -fpermissive -std=c++11 -o test test.cpp -lcurl -lz && \
    PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-strip test

test1:
	cd opm && \
	g++ -std=c++11 -o ../bins/opmtest update.cpp download.cpp install.cpp list.cpp opm.cpp search.cpp remove.cpp -lcurl -lz

clean:
	rm -rf $(NDKDEST)

export:
	cp bins/* ../android_project/OpenPackageManager/app/src/main/res/raw

showsetup:
	@echo "Current environment"
