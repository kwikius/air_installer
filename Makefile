
ifeq ($(OS),Windows_NT)
AIR_TARGET_OS := Windows
else
AIR_TARGET_OS := $(shell uname -s)
endif

ifeq ($(AIR_TARGET_OS),Linux)
CC      = g++
CC1     = gcc
LD      = g++
CFLAGS = -std=c++11
LFLAGS = -static
else
ifeq ($(AIR_TARGET_OS),Windows)

ifndef $(AIR_MINGW_PATH)
$(error "Need to invoke make with path to MINGW like make AIR_MINGW_PATH=/c/MinGW")
endif

CC      = $(AIR_MINGW_PATH)/bin/g++
CC1     = $(AIR_MINGW_PATH)/bin/gcc
LD      = $(AIR_MINGW_PATH)/bin/g++
CFLAGS = -std=c++11
LFLAGS = 
else
$(error "No OS defined")
endif
endif

TARGET = airtool.exe

local_objects = main.o dependency.o platform.o stage.o dependency_arm_none_eabi_gcc.o

objects = $(patsubst %, obj/%, $(local_objects))

all: bin obj | bin/$(TARGET)

bin:
	mkdir -p bin

obj:
	mkdir -p obj

$(objects) : obj/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

bin/$(TARGET) : $(objects)
	$(LD) $(LDFLAGS) -o $@ $(objects)

clean:
	rm -rf obj/*.o bin/$(TARGET)
