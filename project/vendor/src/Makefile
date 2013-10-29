.PHONY: clean ogg vorbis rtaudio dumb

# Version of each library
OGG := libogg-1.3.1
VORBIS := libvorbis-1.3.3
RTAUDIO := rtaudio-4.0.12
DUMB := dumb-0.9.3

default: all

ifeq ("$(OS)","")
OS := $(OSTYPE)
endif
OS := $(OS:mac%=darwin)
OS := $(OS:darwin%=darwin)
OS := $(OS:linux%=linux)
OS := $(OS:Linux%=linux)
OS := $(OS:Windows%=windows)

ifeq ("$(OS)","")
OS := linux
endif

TAR_ARGS := --no-same-owner

SS_LIBEXT := .a
SS_OBJEXT := .o
SS_CC := gcc -O2
SS_CPP := g++
SS_OBJOUT := -o
EMPTY :=
SPACE :=  $(EMPTY) $(EMPTY)
SS_CREATELIB := ar -cr$(SPACE)
SS_CREATEDLL := gcc -dynamic -fPIC -o

ifeq ("$(OS)","darwin")
    SS_MAC := 1
    export SS_MAC
    SS_CONFIG := mac
    LIB_DIR := ../lib/Mac
    JCONFIG := jconfig.mac
    SS_DLLEXT := .dylib

    # http://xquartz.macosforge.org/landing/
    # ln -s /opt/X11/include/X11 /usr/local/include/X11

    MACOSX_DEPLOYMENT_TARGET:=10.7
    export MACOSX_DEPLOYMENT_TARGET
    ifdef SS_64
        SS_CC += -fPIC -m64
        SS_CPP += -fPIC -m64 -mmacosx-version-min=10.7
        LIB_DIR := ../lib/Mac64
    else
        SS_CC += -fPIC -m32
        SS_CPP += -fPIC -m32 -mmacosx-version-min=10.7
    endif

endif

ifeq ("$(OS)","webos")
    ifeq ("$(PalmPDK)","")
        PalmPDK := /opt/PalmPDK
    endif

    PATH := $(PalmPDK)/arm-gcc/bin:$(PATH)


    SS_WEBOS := 1
    export SS_WEBOS
    export PATH
    SS_CONFIG := webos
    LIB_DIR := ../lib/webOS
    #JCONFIG := jconfig.mac
    SS_DLLEXT := .so
    SS_CC := arm-none-linux-gnueabi-gcc -mfloat-abi=softfp -DWEBOS
    SS_RANLIB :=  arm-none-linux-gnueabi-ranlib
    SS_CREATELIB := arm-none-linux-gnueabi-ar -cr$(SPACE)
endif


ifeq ("$(OS)","blackberry")

ifeq ("x$(BLACKBERRY_NDK_ROOT)","x")
default:
	@echo "Please set BLACKBERRY_NDK_ROOT"

endif

    #QNX_HOST := ${BLACKBERRY_NDK_ROOT}/host/win32/x86
    #QNX_TARGET := ${BLACKBERRY_NDK_ROOT}/target/qnx6

    #PATH := ${QNX_HOST}/usr/bin/:$(PATH)

    SS_BLACKBERRY := 1
    export SS_BLACKBERRY

    ifeq ("$(ARCH)","x86")
        SS_ARCH_X86 := 1
        export SS_ARCH_X86

        SS_LIBEXT := -x86.a
        SS_DLLEXT := -x86.so
        SS_CC :=  i486-pc-nto-qnx6.5.0-gcc "-I${QNX_TARGET}/usr/include" -DBLACKBERRY -fpic -fPIC -O2 -fvisibility=hidden
        SS_RANLIB := i486-pc-nto-qnx6.5.0-ranlib
        SS_CREATELIB := i486-pc-nto-qnx6.5.0-ar -cr$(SPACE)
    else
        SS_DLLEXT := .so
        SS_CC :=  arm-unknown-nto-qnx6.5.0eabi-gcc "-I${QNX_TARGET}/usr/include" -DBLACKBERRY -fpic -fPIC -O2 -fvisibility=hidden
        SS_RANLIB := arm-unknown-nto-qnx6.5.0-ranlib
        SS_CREATELIB := arm-unknown-nto-qnx6.5.0eabi-ar -cr$(SPACE)
    endif

    SS_CONFIG := blackberry
    LIB_DIR := ../lib/BlackBerry
    #JCONFIG := jconfig.mac
endif


ifeq ("$(OS)","android")

ifeq ("x$(ANDROID_NDK_ROOT)","x")
default:
	@echo "Please set ANDROID_NDK_ROOT"

endif

    ANDROID_HOST := $(OSTYPE)
    ANDROID_HOST := $(ANDROID_HOST:darwin%=darwin-x86_64)
    ANDROID_HOST := $(ANDROID_HOST:Windows%=windows)

    PATH := $(ANDROID_NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/$(ANDROID_HOST)/arm-linux-androideabi/bin:$(ANDROID_NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/$(ANDROID_HOST)/libexec/gcc/arm-linux-androideabi/4.4.3:$(PATH)

    SS_ANDROID := 1
	export SS_ANDROID
	export PATH
	SS_CONFIG := android
	LIB_DIR := ../lib/Android
	#JCONFIG := jconfig.mac
	SS_DLLEXT := .so
	SDL := ERROR
	SS_CC := gcc \
		"-I$(ANDROID_NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/$(ANDROID_HOST)/lib/gcc/arm-linux-androideabi/4.4.3/include/" \
		"-I$(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/include" \
		"-I$(ANDROID_NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi/include" \
		"-I$(ANDROID_NDK_ROOT)/platforms/android-5/arch-arm/usr/include" \
		-D_LINUX_STDDEF_H \
		-fpic -fvisibility=hidden -mthumb-interwork -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -DANDROID -Wa,--noexecstack -O2 -DNDEBUG -g -c
	SS_RANLIB := "ranlib"
endif



ifeq ("$(wildcard /Applications/Xcode.app/*)","")
    XCODE_DEVELOPER :=  /Developer
else
    XCODE_DEVELOPER :=  /Applications/Xcode.app/Contents/Developer
endif

IPHONE_VER := 6.1
ifneq ("$(VER)","")
    IPHONE_VER := $(VER)
endif

ifeq ("$(OS)","iphoneos")
  	SS_IPHONEOS := 1

  	IPHONE_ARCH := armv6
	ifeq ("$(ARM)","v7")
	    IPHONE_ARCH := armv7
	    ARCH_EXT := -v7
	endif

    SS_LIBEXT := .iphoneos$(ARCH_EXT).a
    export SS_IPHONEOS
    export SS_IPHONE
    JCONFIG := jconfig.mac
    SS_CONFIG := iphoneos
    SS_IPHONE := 1
    LIB_DIR := ../lib/iPhone
    SS_CC := $(XCODE_DEVELOPER)/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc  \
        -DIPHONE=1 -D__IPHONEOS__=1 -O2 \
        -arch $(IPHONE_ARCH) -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings \
        -Wreturn-type -Wunused-variable -fno-stack-protector  \
        -I$(XCODE_DEVELOPER)/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(IPHONE_VER).sdk/usr/include/ \
        -isysroot $(XCODE_DEVELOPER)/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(IPHONE_VER).sdk
endif


ifeq ("$(OS)","iphonesim")
    SS_IPHONESIM := 1
    SS_LIBEXT := .iphonesim.a
    export SS_IPHONESIM
    export SS_IPHONE
    JCONFIG := jconfig.mac
    SS_CONFIG := iphonesim
    LIB_DIR := ../lib/iPhone
    SS_IPHONE := 1
    SS_CC := $(XCODE_DEVELOPER)/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc  \
        -DIPHONE=1 -D__IPHONESIM__=1 -O2 \
        -arch i386 -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings \
        -Wreturn-type -Wunused-variable  -fno-stack-protector \
        -I$(XCODE_DEVELOPER)/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneOS$(IPHONE_VER).sdk/usr/include/ \
        -isysroot $(XCODE_DEVELOPER)/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$(IPHONE_VER).sdk
endif

ifeq ("$(OS)","linux")
    SS_CONFIG := linux
    SS_LINUX := 1
    export SS_LINUX
    ifeq ($(shell uname -a | cut -f 2 -d ' '),raspberrypi)
        SS_RPI := 1
        SS_CC += -fPIC -DRASPBERRYPI
        LIB_DIR := ../lib/RPi
    else
        ifdef SS_64
            SS_CC += -fPIC -m64
            LIB_DIR := ../lib/Linux64
        else
            SS_CC += -fPIC -m32
            LIB_DIR := ../lib/Linux
        endif
    endif
    export SS_ABI
    JCONFIG := jconfig.linux
    SS_DLLEXT := .dso
endif


ifeq ("$(OS)","windows")
    SS_WINDOWS := 1
    export SS_WINDOWS
    SS_CONFIG := windows
    ifdef SS_WINRT
        LIB_DIR := ../lib/WinRTx86
        SS_CC := cl.exe -nologo -MT -DWINAPI_FAMILY=WINAPI_FAMILY_APP /WX- /fp:precise -Z7 -O2 -DWINRT
    else
        LIB_DIR := ../lib/Windows
        SS_CC := cl.exe -nologo -MT -Z7 -O2 -DWIN32
    endif
    SS_LIBEXT := .lib
    JCONFIG := jconfig.vc
    SS_OBJEXT := .obj
    SS_OBJOUT := -Fo
    SS_CREATELIB := lib -out:
    SS_DLLEXT := .dll
endif


ifeq ("$(OS)","winrt")
    SS_WINDOWS := 1
    export SS_WINDOWS
    SS_WINRT := 1
    export SS_WINRT
    SS_CONFIG := winrt
    LIB_DIR := ../lib/WinRTx86
    SS_LIBEXT := .lib
    JCONFIG := jconfig.vc
    SS_CC := cl.exe -nologo -Z7 -O2 -DWIN32 -DWINAPI_FAMILY=2 "-Ic:\Program Files (x86)\Windows Kits\8.0\Include\shared" /Zl
    SS_OBJEXT := .obj
    SS_OBJOUT := -Fo
    SS_CREATELIB := lib -out:
    SS_DLLEXT := .dll
endif

# --- EXPORTS --------------------

export SS_CONFIG
export SS_CC
export SS_CPP
export SS_OBJOUT
export SS_OBJEXT
export SS_CREATELIB
export SS_LIBEXT
export SS_DLLEXT
export SS_CREATEDLL

# --- ALL ------------------------

all: clean
all: setup

all: dumb

ifndef SS_WEBOS
ifndef SS_WINRT
all: ogg
all: vorbis
endif
endif

ifndef SS_IPHONE
ifndef SS_ANDROID
all: rtaudio
endif
endif

# --- SETUP ------------------------

setup:
	mkdir -p $(LIB_DIR)

# --- OGG ------------------------

ogg: $(OGG)

$(OGG):
	tar xvzf tars/$(OGG).tgz $(TAR_ARGS)
	cp configs/ogg-config_types.h $(OGG)/include/ogg/config_types.h
	cp configs/ogg-makefile $(OGG)/Makefile
	cd $(OGG); unset MAKEFLAGS; make
	cp $(OGG)/libogg$(SS_LIBEXT) $(LIB_DIR)
	mkdir -p ../include/ogg
	cp $(OGG)/include/ogg/ogg.h ../include/ogg
	cp configs/ogg-config_types.h ../include/ogg/config_types.h
	cp $(OGG)/include/ogg/os_types.h ../include/ogg

clean::
	rm -rf $(OGG)


# --- VORBIS ------------------------

vorbis: $(VORBIS)

$(VORBIS):
	tar xvzf tars/$(VORBIS).tgz $(TAR_ARGS)
	cp configs/vorbis-makefile $(VORBIS)/Makefile
	cp patches/vorbis/os.h $(VORBIS)/lib
	cd $(VORBIS); make
	cp $(VORBIS)/libvorbis$(SS_LIBEXT) $(LIB_DIR)
	mkdir -p ../include/vorbis
	cp $(VORBIS)/include/vorbis/vorbisfile.h ../include/vorbis
	cp $(VORBIS)/include/vorbis/codec.h ../include/vorbis
	cp $(VORBIS)/include/vorbis/vorbisenc.h ../include/vorbis

clean::
	rm -rf $(VORBIS)


# --- RTAUDIO ------------------------

rtaudio: $(RTAUDIO)

$(RTAUDIO):
	tar xvzf tars/$(RTAUDIO).tgz $(TAR_ARGS)
	cp configs/rtaudio-makefile $(RTAUDIO)/Makefile
	cd $(RTAUDIO); make
	cp $(RTAUDIO)/librtaudio$(SS_LIBEXT) $(LIB_DIR)
	mkdir -p ../include/rtaudio
	cp $(RTAUDIO)/RtAudio.h ../include
	cp $(RTAUDIO)/RtError.h ../include

clean::
	rm -rf $(RTAUDIO)


# --- DUMB ------------------------

dumb: $(DUMB)

$(DUMB):
	tar xvzf tars/$(DUMB).tgz $(TAR_ARGS)
	cp configs/dumb-makefile $(DUMB)/Makefile
	cd $(DUMB); make
	cp $(DUMB)/libdumb$(SS_LIBEXT) $(LIB_DIR)
	mkdir -p ../include
	cp $(DUMB)/include/dumb.h ../include

clean::
	rm -rf $(DUMB)
