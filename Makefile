all: build

CFLAGS := *.c -lm -L../lib ../lib/libraylib.a -I../lib/ -Os
this_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown OS')
target ?= $(this_OS)

ifeq ($(target),Linux)
    CFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -o ../cnake
endif
ifeq ($(target),Darwin)
    CFLAGS += -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL -o ../cnake
endif
ifeq ($(target),Windows_NT)
	CFLAGS += -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -o ../cnake.exe
endif

ifeq ($(target),Windows_NT)
	compiler := x86_64-w64-mingw32-gcc
else
	compiler := gcc
endif

fetch_raylib:
ifeq ($(target),Linux)
	wget -q https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz
	tar -xf ./raylib-5.0_linux_amd64.tar.gz
	mv ./raylib-5.0_linux_amd64 ./raylib
	rm ./raylib-5.0_linux_amd64.tar.gz
endif
ifeq ($(target),Darwin)
	wget -q https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_macos.tar.gz
	tar -xf ./raylib-5.0_macos.tar.gz
	mv ./raylib-5.0_macos ./raylib
	rm ./raylib-5.0_macos.tar.gz
endif
ifeq ($(target),Windows_NT)
	wget -q https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip
	unzip -qq raylib-5.0_win64_mingw-w64.zip
	mv ./raylib-5.0_win64_mingw-w64 ./raylib
	rm ./raylib-5.0_win64_mingw-w64.zip
endif

	mv ./raylib/lib/libraylib.a ./lib/
	mv ./raylib/include/raylib.h ./lib/
	rm -rf ./raylib

build:
	([ ! -e ./lib/libraylib.a ] || [ ! -e ./lib/raylib.h ]) && $(MAKE) fetch_raylib || :
	(cd src && $(compiler) $(CFLAGS))

clean:
	([ -f lib/libraylib.a ] || [ -f lib/raylib.h ]) && rm ./lib/* || :
