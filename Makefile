

UNAME := $(shell uname -s)

ifneq ($(findstring Linux, $(UNAME)),)
OS_SHELL = Linux
CXX = g++
else
ifneq ($(findstring CYGWIN, $(UNAME)),)
OS_SHELL = Cygwin
LDFLAGS = -static
else
ifneq ($(findstring MINGW,$(UNAME)),)
OS_SHELL = MinGW
LDFLAGS = -static
else
$(error "Cant deduce OS Shell that make is running from.")
endif
endif
endif

CXX = g++
LD = $(CXX)
CXXFLAGS = -std=c++11

TARGET = airtool.exe

local_objects = main.o dependency.o platform.o stage.o dependency_arm_none_eabi_gcc.o \
file_utils.o config.o dependency_list.o simple_dependency.o

objects = $(patsubst %, obj/%, $(local_objects))

all: bin obj | bin/$(TARGET)

bin:
	mkdir -p bin

obj:
	mkdir -p obj

$(objects) : obj/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/$(TARGET) : $(objects)
	$(LD) $(LDFLAGS) -o $@ $(objects)

execute: bin/$(TARGET)
	bin/$(TARGET) --bin-dir $$HOME/ap_bin --lib-dir $$HOME/ap_lib

clean:
	rm -rf obj/*.o bin/$(TARGET)
