CC = clang
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -std=gnu99 -O3 -m64 -fPIC -I source
LFLAGS = -shared
SOURCES = Gamepad_private

ifeq ($(OS),Windows_NT)
    SOURCES += Gamepad_windows_dinput
    SOURCES += Gamepad_windows_mm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        SOURCES += Gamepad_linux
    endif
    ifeq ($(UNAME_S),Darwin)
        SOURCES += Gamepad_macosx
    endif
endif

all : library

library : clean 
	mkdir build
	$(foreach file, $(SOURCES), $(CC) $(CFLAGS) -c -o build/$(file).o source/gamepad/$(file).c;)
	$(CC) $(LFLAGS) -o build/libstem_gamepad.so $(addsuffix .o, $(addprefix build/,$(SOURCES)))

clean :
	rm -rf build
