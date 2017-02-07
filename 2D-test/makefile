APPNAME=ebook
MAINFILE=main.c

PREFIX_PATH=.

TOOLCHAIN_LOC=/opt
INCLUDE_PATH=-I./inc -I./include/ -I.

LDLIB=-lm -lpthread -lrt
HELPCMD=

ifndef PLATFORM
	PLATFORM=AmLogic
#	HELPCMD=exit 2
endif

ifndef CONFIGURATION
	CONFIGURATION=rtm
# HELPCMD=exit 2
endif

NODEPS:=showhelp clean install strip
#########PLATFORM########
#
# SUPPLEMENT_PATH为某个平台特殊需要的库和路径。
# 不在通用中。
#
#############

ifeq "$(PLATFORM)" "AmLogic"
#CFLAG+=
CROSS=$(TOOLCHAIN_LOC)/gnutools/arc2.3-p0/uclibc-4.2.1/bin/arc-linux-uclibc-
INCLUDE_PATH+=-I$(TOOLCHAIN_LOC)/gnutools/arc2.3-p0/uclibc-4.2.1/include
MACRO_DEF+=-DPLATFORM_AMLOGIC
SUPPLEMENT_PATH=-L$(TOOLCHAIN_LOC)/gnutools/arc2.3-p0/uclibc-4.2.1/lib 
#-lcmem

endif

###########################
CFLAGS+=-fPIC -pipe -fstack-check 

CC=$(CROSS)gcc
STRIP=$(CROSS)strip
STRIP_LIB= 

ifeq "$(IGNORE_WARNING)" "yes"
CFLAGS+=-w
else
CFLAGS+=-Wall
endif

ifeq "$(CONFIGURATION)" "chk"
CFLAGS+=-g 
LDLIB+=-lefence
MACRO_DEF+=-D_DEBUG=1
endif

ifeq "$(CONFIGURATION)" "fre"
CFLAGS+=-O3
endif

ifeq "$(CONFIGURATION)" "rtm"
CFLAGS+=-O3
MACRO_DEF+=-DRELEASE_TO_MARKET
endif

OBJ_PREFIX_TEMP=$(PREFIX_PATH)/temp
BIN_PREFIX_PATH=$(PREFIX_PATH)/bin

OBJ_PREFIX=$(OBJ_PREFIX_TEMP)/$(CONFIGURATION)/$(PLATFORM)/
BIN_PREFIX=$(BIN_PREFIX_PATH)/$(CONFIGURATION)/$(PLATFORM)/

TARGET_ROOT=TARGET/$(PLATFORM)/$(CONFIGURATION)
TARGET_SYSTEM=$(TARGET_ROOT)/DK_System
TARGET_SYSTEM_LIB=$(TARGET_SYSTEM)/lib

INC_FLAGS=$(INCLUDE_PATH) $(MACRO_DEF)
LDFLAGS=-L./bin/LIB_$(PLATFORM) $(SUPPLEMENT_PATH)


# alsa database comserv update widget dkdevice tdevice tdevice/kindle test
VPATH =.:./alsa:./database:./comserv:./update:./widget:./dkdevice:./tdevice:./test:./code
#TODO::以后改成函数
sources=
sources1=$(wildcard ./test/*.c)
sources11=$(notdir $(sources1))
sources +=$(sources11)

sources2=$(wildcard ./code/*.c)
sources22=$(notdir $(sources2))
sources +=$(sources22)

sources3=$(wildcard ./database/*.c)
sources33=$(notdir $(sources3))
sources +=$(sources33)

sources4=$(wildcard ./comserv/*.c)
sources44=$(notdir $(sources4))
sources +=$(sources44)

sources5=$(wildcard ./update/*.c)
sources55=$(notdir $(sources5))
sources +=$(sources55)

sources6=$(wildcard ./widget/*.c)
sources66=$(notdir $(sources6))
sources +=$(sources66)

sources7=$(wildcard ./dkdevice/*.c)
sources77=$(notdir $(sources7))
sources +=$(sources77)

sources8=$(wildcard ./tdevice/*.c)
sources88=$(notdir $(sources8))
sources +=$(sources88)

objs  =$(addsuffix .o, $(basename $(sources)))
objects =$(addprefix $(OBJ_PREFIX),$(objs))
sourcesd=$(addsuffix .d, $(basename $(objects)))

EXECUTABLE=$(CC) $(CFLAGS) $(objects) -o $@ $(LDFLAGS) $(LDLIB) 

ifeq "$(CodeCoverage)" "true"
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

ifeq "$(CONFIGURATION)" "rtm"
EXECUTABLE+=; $(STRIP) $@
STRIP_LIB=$(STRIP) TARGET/$(PLATFORM)/$(CONFIGURATION)/DK_System/lib/*.so*
endif

	
$(BIN_PREFIX)$(APPNAME):showhelp $(objects)
	$(EXECUTABLE)	

$(OBJ_PREFIX)%.o :%.c $(OBJ_PREFIX)%.d
	@echo "COMPILING "$<  >&2;\
	$(CC)  $(CFLAGS) $(INC_FLAGS) -c $< -o $(addsuffix .o, $(basename $@)) 


all: showhelp clean install strip
	
.PHONY : cleand

clean : showhelp cleand
	rm $(BIN_PREFIX)$(APPNAME) $(objects) *~ -rf

cleand:
	rm *.d  $(sourcesd) -rf
		
install:$(BIN_PREFIX)$(APPNAME)
	cp $< /nfsroot/wangjf/rootfs_a3/usr/bin -rf
	cp src/ReleasePackageTemplate/$(PLATFORM)/*  TARGET/$(PLATFORM)/$(CONFIGURATION)/ -rf
	cp $(BIN_PREFIX)$(APPNAME) $(TARGET_SYSTEM)/bin
	cp bin/LIB_$(PLATFORM)/*   $(TARGET_SYSTEM_LIB) -P
	mv $(TARGET_SYSTEM_LIB)/libcurl.so $(TARGET_SYSTEM_LIB)/libcurl.so.4
	mv $(TARGET_SYSTEM_LIB)/libcv.so $(TARGET_SYSTEM_LIB)/libcv.so.1
	mv $(TARGET_SYSTEM_LIB)/libcvaux.so $(TARGET_SYSTEM_LIB)/libcvaux.so.1
	mv $(TARGET_SYSTEM_LIB)/libcxcore.so $(TARGET_SYSTEM_LIB)/libcxcore.so.1
	mv $(TARGET_SYSTEM_LIB)/libdjvulibre.so $(TARGET_SYSTEM_LIB)/libdjvulibre.so.21
	mv $(TARGET_SYSTEM_LIB)/libfreeimage.so $(TARGET_SYSTEM_LIB)/libfreeimage.so.3
	mv $(TARGET_SYSTEM_LIB)/libfreetype.so $(TARGET_SYSTEM_LIB)/libfreetype.so.6
	mv $(TARGET_SYSTEM_LIB)/libhighgui.so $(TARGET_SYSTEM_LIB)/libhighgui.so.1
	mv $(TARGET_SYSTEM_LIB)/libiconv.so $(TARGET_SYSTEM_LIB)/libiconv.so.2	
	mv $(TARGET_SYSTEM_LIB)/libjbig2dec.so $(TARGET_SYSTEM_LIB)/libjbig2dec.so.0
	mv $(TARGET_SYSTEM_LIB)/libjson.so $(TARGET_SYSTEM_LIB)/libjson.so.0
	mv $(TARGET_SYSTEM_LIB)/libml.so $(TARGET_SYSTEM_LIB)/libml.so.1
	mv $(TARGET_SYSTEM_LIB)/libopenjpeg.so $(TARGET_SYSTEM_LIB)/libopenjpeg.so.2
	mv $(TARGET_SYSTEM_LIB)/libtiff.so $(TARGET_SYSTEM_LIB)/libtiff.so.3
	mv $(TARGET_SYSTEM_LIB)/libxml2.so $(TARGET_SYSTEM_LIB)/libxml2.so.2
	mv $(TARGET_SYSTEM_LIB)/libzip.so $(TARGET_SYSTEM_LIB)/libzip.so.1
	mv $(TARGET_SYSTEM_LIB)/libpcre.so $(TARGET_SYSTEM_LIB)/libpcre.so.0
	mv $(TARGET_SYSTEM_LIB)/libpcreposix.so $(TARGET_SYSTEM_LIB)/libpcreposix.so.0
	$(RENAMECOMAND)	
	find  $(TARGET_ROOT)/  -type l -exec rm -rf {} \;
	find  $(TARGET_ROOT)/  -type d -name "\.svn" -print0 | xargs -0 rm -rf

strip:
	$(STRIP_LIB)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS)))) 
    sinclude $(objects:.o=.d)
endif 


$(OBJ_PREFIX)%.d: %.c
	@set -e; \
	rm -f $@;\
	$(CC)  -MM $(INC_FLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$ ;\

showhelp:
	@echo  "#####################################################################" 	
	@echo  "# Please specify the PLATFORM and CONFIGURATION in your make command#"
	@echo  "#                                                                   #"	
	@echo  "# make PLATFORM={Aigo|Kindle|Samsung|AmLogic}   CONFIGURATION={chk|fre|rtm} #"
	@echo  "#        Supported CMD: {all|clean|strip|install|showhelp}          #"
	@echo  "#                        Welcome to code jail                       #"
	@echo  "#####################################################################" 	
	@`$(HELPCMD)`
	@echo "      PLATFORM      =$(PLATFORM)" 
	@echo "      CONFIGURATION =$(CONFIGURATION)" 	
	@sleep 3
