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
NDKPLATFORM?=android-9
BUILDSYSTEM=linux-x86_64
NDKBOOST=/home/amoghbl1/apps/android_ndk/android-ndk-r10/

all: | maketoolchain build export
	@echo Built Successfully 

maketoolchain:
	bash $(NDK)/build/tools/make-standalone-toolchain.sh --platform=$(NDKPLATFORM) --install-dir=$(NDKDEST) --arch=$(HOSTARCH) --system=$(BUILDSYSTEM)

build: | buildboost moveboost configure compile
	@echo "Type 'build' successfuly built"

buildboost:
	cd Boost-for-Android && ./build-android.sh $(NDKBOOST) 

moveboost:
	cd Boost-for-Android && cp -r build/include/boost-1_53/boost/ ../includes/boost/

buildcurl:
	cd curl && \
	PATH=$(NDKDEST)/bin:$(PATH) ./configure --host=arm-linux-androideabi && \
	PATH=$(NDKDEST)/bin:$(PATH) make

movecurl:
	rm -rf include/curl
	cd curl && cp -r include/curl/ ../includes/
	cd includes/curl && sed -i "s/define CURL_SIZEOF_LONG           8/define CURL_SIZEOF_LONG           4/g" curlbuild.h.dist
	@echo "Fixing sizeof problem"

#configure:
#	cd opm && PATH=$(NDKDEST)/bin:$(PATH) LUA_CFLAGS="-DLUA_USE_POSIX" LDFLAGS="-static" \
#	ac_cv_linux_vers=2 CC=$(PREFIXT)-gcc CXX=$(PREFIXT)-g++ LD=$(PREFIXT)-ld RANLIB=$(PREFIXT)-ranlib AR=$(PREFIXT)-ar STRIP=$(PREFIXT)-strip \
#	./configure $(HOSTPARM) --without-zenmap --with-liblua=included --with-libpcap=internal --with-pcap=linux --enable-static --prefix=$(ANDROIDDEST) $(OPENSSLOPT)

compile:
	cd opm && \
	PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-g++ -I../includes/ -pie -fPIE -fpermissive  -std=c++11 update.cpp download.cpp install.cpp -o installer && \
	PATH=$(NDKDEST)/bin:$(PATH) $(PREFIXT)-strip installer

clean:
	rm -rf $(NDKDEST)

export:
	mkdir -p bins

showsetup:
	@echo "Current environment"
