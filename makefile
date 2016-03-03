########################################################################
# 目录定义
########################################################################
inc_dir         = include
src_dir         = src
lib_dir         = lib
bin_dir         =

########################################################################
# 输出
########################################################################
# app, static, shared
target_type     = static
target          = takisy

########################################################################
# 外部库定义
########################################################################
ext_inc_dir     = /usr/local/include/freetype2
ext_lib_dir     =
ext_lib         =

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
cxxflags        = -g $(LEVEL) --std=gnu++11 -Wall -Werror -Wno-array-bounds
ldflags         =

########################################################################
# 相关目录
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
	@echo 'g++.o   $@'
	@$(cxx) $(cxxflags) $(inc_dir) $(ext_inc_dir) -c "$<" -o $@

-include $(dpds)

$(dpds): %.d: %.cpp
	@echo 'g++.d   $@'
	@$(cxx) $(cxxflags) $(inc_dir) $(ext_inc_dir) -MM "$<" | \
    sed 's/^$(notdir $(basename $@)).o/$(subst /,\/,$(basename $@)).o $(subst /,\/,$(basename $@)).d/' > $@

$(target): $(objs)
ifeq ($(target_type),app)
	@echo 'g++     $@'
	@$(cxx) $^ $(ext_lib_dir) $(ext_lib) $(ldflags) -o $@
	@$(strip) $@
else
ifeq ($(target_type),static)
	@echo 'ar      $@'
	@$(ar) rcs $@ $^
else
ifeq ($(target_type),shared)
	@echo 'g++.so  $@'
	@$(cxx) $^ $(ext_lib_dir) $(ext_lib) -shared -o $@
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

