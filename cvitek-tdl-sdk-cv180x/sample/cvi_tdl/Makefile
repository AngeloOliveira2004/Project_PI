CHIP ?= cv1835
SDK_VER ?= 64bit

CROSS_COMPILE_RISCV64_MUSL ?= riscv64-unknown-linux-musl-

ifeq ($(SDK_VER), 64bit)
ARCH=arm64
CROSS_COMPILE = $(CROSS_COMPILE_64)
CFLAGS += -march=armv8-a
else ifeq ($(SDK_VER), musl_riscv64)
ARCH=riscv64
CROSS_COMPILE = $(CROSS_COMPILE_RISCV64_MUSL)
CFLAGS += -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d 
else
$(error "Unknown SDK_VER: $(SDK_VER")")
endif

#####################################################
# Gcc Compiler
#####################################################
CC = $(CROSS_COMPILE)gcc

#####################################################
# Middleware stuffs
#####################################################
MW_LIB_PATH = $(MW_PATH)/lib
MW_INC_PATH = $(MW_PATH)/include
MW_PANEL_INC_PATH += ${MW_PATH}/include/panel
MW_SAMPLE_PATH = $(MW_PATH)/sample/common
MW_3RD_LIB_PATH = $(MW_LIB_PATH)/3rd
MW_PANEL_BOARD_INC_PATH := $(MW_PATH)/include

MW_VERSION = v2
CFLAGS += -D_MIDDLEWARE_V2_

MW_LIBS = -lini -lsns_full -lsample -lisp -lvdec -lvenc -lawb -lae -laf -lcvi_bin -lcvi_bin_isp -lmisc -lisp_algo -lsys -lvpu
MW_ISP_INC_PATH = $(MW_INC_PATH)/isp/cv180x
CFLAGS += -DCV180X

#####################################################
# TDL SDK stuffs
#####################################################
SDK_ROOT_PATH ?= $(abspath ../..)
SDK_LIB_PATH = $(SDK_ROOT_PATH)/lib
SDK_INC_PATH = $(SDK_ROOT_PATH)/include
SDK_TDL_INC_PATH = $(SDK_ROOT_PATH)/include/cvi_tdl
SDK_APP_INC_PATH = $(SDK_ROOT_PATH)/include/cvi_tdl_app
SDK_SAMPLE_INC_PATH = $(SDK_ROOT_PATH)/sample/3rd/include
SDK_SAMPLE_UTILS_PATH = $(SDK_ROOT_PATH)/sample/utils
SDK_TDL_LIBS = -lcvi_tdl

#####################################################
# Opencv
#####################################################
OPENCV_INC_PATH = $(SDK_ROOT_PATH)/sample/3rd/opencv/include
OPENCV_LIB_PATH = $(SDK_ROOT_PATH)/sample/3rd/opencv/lib
OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_imgcodecs

#####################################################
# TPU
#####################################################
TPU_LIB_PATH = $(TPU_PATH)/lib
TPU_LIBS = -lcnpy  -lcvikernel  -lcvimath  -lcviruntime  -lz -lm

#####################################################
# IVE SDK
#####################################################
IVE_SDK_LIB_PATH = $(IVE_PATH)/lib
IVE_SDK_INC_PATH = $(IVE_PATH)/include
IVE_LIBS = -lcvi_ive_tpu
CFLAGS += -DUSE_TPU_IVE


#####################################################
# RTSP stuffs
#####################################################
RTSP_LIB_PATH = $(SDK_ROOT_PATH)/sample/3rd/rtsp/lib
RTSP_INC_PATH = $(SDK_ROOT_PATH)/sample/3rd/rtsp/include/cvi_rtsp
RTSP_LIBS = -lcvi_rtsp

#####################################################
# STB
#####################################################
STB_INC_PATH = $(SDK_ROOT_PATH)/sample/3rd/stb/include

CFLAGS += -std=gnu11 -Wno-pointer-to-int-cast -fsigned-char -Werror=all -Wno-format-truncation -fdiagnostics-color=always -s -lpthread -latomic

SRCS := $(wildcard $(PWD)/*.c)
OBJS := $(SRCS:%.c=%.o)

SAMPLE_BASE_LIBS = -L$(TPU_LIB_PATH) $(TPU_LIBS) -L$(IVE_SDK_LIB_PATH) $(IVE_LIBS) -L$(SDK_LIB_PATH) $(SDK_TDL_LIBS)
SAMPLW_MW_LIB = -L$(MW_LIB_PATH) -L$(MW_3RD_LIB_PATH) $(MW_LIBS)
SAMPLE_OPENCV_LIB = -L$(OPENCV_LIB_PATH) $(OPENCV_LIBS)

SAMPLE_VI_LIBS = $(SAMPLW_MW_LIB) $(SAMPLE_OPENCV_LIB) $(SAMPLE_BASE_LIBS) -L$(RTSP_LIB_PATH) $(RTSP_LIBS)

CFLAGS += -I$(SDK_INC_PATH) \
          -I$(SDK_TDL_INC_PATH) \
          -I$(SDK_APP_INC_PATH) \
          -I$(SDK_SAMPLE_INC_PATH) \
		  -I$(SDK_SAMPLE_UTILS_PATH) \
          -I$(RTSP_INC_PATH) \
          -I$(IVE_SDK_INC_PATH) \
          -I$(OPENCV_INC_PATH) \
          -I$(STB_INC_PATH) \
		  -I$(MW_SAMPLE_PATH) \
          -I$(MW_ISP_INC_PATH) \
		  -I$(MW_PANEL_INC_PATH) \
		  -I$(MW_PANEL_BOARD_INC_PATH) \
		  -I$(MW_INC_PATH) \
		  -I$(MW_INC_PATH)/linux \

TARGETS_VI_SAMPLE := sample_vi_fd

TARGETS = $(TARGETS_VI_SAMPLE) \

.PHONY : all clean

all: $(TARGETS)

clean:
	rm -f $(OBJS) $(TARGETS)

$(PWD)/%.o: $(PWD)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(SDK_ROOT_PATH)/sample/utils/%.o: $(SDK_ROOT_PATH)/sample/utils/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

sample_vi_%: $(PWD)/sample_vi_%.o \
			 $(SDK_ROOT_PATH)/sample/utils/sample_utils.o \
			 $(SDK_ROOT_PATH)/sample/utils/middleware_utils.o \
			 $(SAMPLE_COMMON_FILE)
	$(CC) $(CFLAGS) $(SAMPLE_VI_LIBS) -o $@ $^