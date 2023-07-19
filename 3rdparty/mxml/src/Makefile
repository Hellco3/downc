###############################################################################
#
# A smart Makefile template for GNU/LINUX programming
#
# Author: PRC (ijkxyz AT msn DOT com)
# Date:   2011/06/17
#
# Usage:
#   $ make           Compile and link (or archive)
#   $ make clean     Clean the objectives and target.
###############################################################################

CROSS_COMPILE =
OPTIMIZE := -O0
WARNINGS := -Wall 
DEFS     := 
EXTRA_CFLAGS := -g
LIBS := 
INC_DIR   = ../include 
SRC_DIR   = 
OBJ_DIR   = ./obj
EXTRA_SRC = 
EXCLUDE_FILES = 

SUFFIX       = c cpp cc cxx
TARGET       := $(BUILD_HOME)/lib/libmxml.so
TARGET_TYPE  := so
TARGET_AR       := $(BUILD_HOME)/lib/libmxml_static.a
TARGET_TYPE_AR  := ar

#####################################################################################
#  Do not change any part of them unless you have understood this script very well  #
#  This is a kind remind.                                                           #
#####################################################################################

#FUNC#  Add a new line to the input stream.
define add_newline
$1

endef

#FUNC# set the variable `src-x' according to the input $1
define set_src_x
src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_obj_x
obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef

#VAR# Get the uniform representation of the object directory path name
ifneq ($(OBJ_DIR),)
prefix_objdir  = $(shell echo $(OBJ_DIR)|sed 's:\(\./*\)*::')
prefix_objdir := $(filter-out /,$(prefix_objdir)/)
endif

GCC      := $(CROSS_COMPILE)gcc
G++      := $(CROSS_COMPILE)g++
SRC_DIR := $(sort . $(SRC_DIR))
inc_dir = $(foreach d,$(sort $(INC_DIR) $(SRC_DIR)),-I$d)

#--# Do smart deduction automatically
$(eval $(foreach i,$(SUFFIX),$(call set_src_x,$i,$(SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_obj_x,$i,$(src-$i),$(prefix_objdir))))
$(eval $(foreach f,$(EXTRA_SRC),$(call add_newline,vpath $(notdir $f) $(dir $f))))
$(eval $(foreach d,$(SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))

all_objs = mxml-attr.o mxml-entity.o mxml-file.o mxml-get.o \
mxml-index.o mxml-node.o mxml-search.o mxml-set.o \
mxml-private.o mxml-string.o 
all_srcs = $(foreach i,$(SUFFIX),$(src-$i))

CFLAGS       = $(EXTRA_CFLAGS) $(WARNINGS) $(OPTIMIZE) $(DEFS)
GCCFLAGS       = $(OPTIMIZE) $(DEFS)
TARGET_TYPE := $(strip $(TARGET_TYPE))

ifeq ($(filter $(TARGET_TYPE),so ar app),)
$(error Unexpected TARGET_TYPE `$(TARGET_TYPE)')
endif

ifeq ($(TARGET_TYPE),so)
 CFLAGS  += -fpic -shared $(inc_dir)
 LDFLAGS += -shared
else
 LDFLAGS += -g
endif

PHONY = all clean

all: $(TARGET) $(TARGET_AR)

#ifeq ($(TARGET_TYPE),ar)
$(TARGET_AR): AR := $(CROSS_COMPILE)ar
$(TARGET_AR): $(all_objs)
	rm -f $@
	$(AR) rcvs $@ $(all_objs)
#else
$(TARGET): LD = $(if $(strip $(src-cpp) $(src-cc) $(src-cxx)),$(G++),$(GCC))
$(TARGET): $(all_objs)
	@echo $(LD)
	$(LD) $(LDFLAGS) $(all_objs) -o $@ $(LIBS)
#endif

mxml-attr.o: mxml-attr.c 
mxml-entity.o: mxml-entity.c 
mxml-file.o: mxml-file.c
mxml-get.o: mxml-get.c
mxml-index.o: mxml-index.c
mxml-node.o: mxml-node.c
mxml-search.o: mxml-search.c
mxml-set.o: mxml-set.c
mxml-private.o: mxml-private.c
mxml-string.o: mxml-string.c

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi

clean:
	rm -f $(all_objs) $(TARGET) $(TARGET_AR)

-include $(patsubst %.o,%.o.d,$(all_objs))

.PHONY: $(PHONY)
