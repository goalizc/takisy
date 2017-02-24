########################################################################
# 目录定义
########################################################################
inc_dir         = include
src_dir         = src
lib_dir         = lib
bin_dir         = lib

########################################################################
# 输出
########################################################################
# app, static, shared
target_type     = static
target          = takisy

########################################################################
# 外部依赖
########################################################################
dependency      =

########################################################################
# 外部库定义
########################################################################
ext_inc_dir     = /usr/local/include/freetype2
ext_lib_dir     = /z/mingw/msys/1.0/local/lib
ext_lib         = freetype ws2_32 gdi32 imm32

########################################################################
# 编译相关
########################################################################
cc              = gcc
cxx             = g++
ar              = ar
strip           = strip
rm              = rm -rf
mkdir           = mkdir -p

ccflags         =
cxxflags        = -O3 --std=gnu++11 -fopenmp -Wall -Werror -Wno-array-bounds
ldflags         =

########################################################################
# 头文件包含与链接库
########################################################################
inc_dir        := $(addprefix -I,$(inc_dir))
ext_inc_dir    := $(addprefix -I,$(ext_inc_dir))
ext_lib_dir    := $(addprefix -L,$(ext_lib_dir))
ext_lib        := $(addprefix -l,$(ext_lib))

########################################################################
# 相关文件
########################################################################
srcs = $(shell find $(src_dir) -type f -name "*.cpp" -print)
objs = $(srcs:%.cpp=%.o)
dpds = $(srcs:%.cpp=%.d)

########################################################################
# 规则定义
########################################################################
ifeq ($(target_type),app)
target := $(bin_dir)/$(target)
else
ifeq ($(target_type),static)
target := $(lib_dir)/lib$(target).a
else
ifeq ($(target_type),shared)
target := $(bin_dir)/lib$(target).dll
endif # shared
endif # static
endif # app

all: prebuild $(target) postbuild

$(objs): %.o: %.cpp
	@echo 'cxx.o   $@'
	@$(cxx) $(cxxflags) $(inc_dir) $(ext_inc_dir) -c "$<" -o $@

-include $(dpds)

$(dpds): %.d: %.cpp
	@echo 'cxx.d   $@'
	@$(cxx) $(cxxflags) $(inc_dir) $(ext_inc_dir) -MM "$<" | \
	sed 's?^$(notdir $*)\.o?$*.o $*.d?g' > $@

$(target): $(objs)
ifeq ($(target_type),app)
	@echo 'cxx     $@'
	@$(cxx) $^ $(ldflags) $(ext_lib_dir) $(ext_lib) -o $@
	@$(strip) $@
else
ifeq ($(target_type),static)
	@echo 'ar      $@'
	@$(ar) rcs $@ $^
else
ifeq ($(target_type),shared)
	@echo 'cxx.so  $@'
	@$(cxx) $^ -shared $(ldflags) $(ext_lib_dir) $(ext_lib) -o $@
endif # shared
endif # static
endif # app

clean:
	@$(rm) $(dpds) $(objs) $(target)

rebuild: clean
	@make

# Add PREBUILD event code here and add this target to target 'all'.
prebuild:

# Add POSTBUILD event code here and add this target to target 'all'.
postbuild:

help:
	@echo 'Usage: make [target]'
	@echo 'Targets:'
	@echo ' *all                default target, same as "make"'
	@echo '  clean              clean this project'
	@echo '  rebuild            clean this project and then build it'
	@echo '  prebuild           do prebuild event'
	@echo '  postbuild          do postbuild event'
