.PHONY: all upx wasm js run test testsoft test60 dbg grind clean

name = HamPurg
SHELL := /bin/bash

all:
	gcc main.c -Ofast -lSDL2 -lGLESv2 -lEGL -lm -o $(name)_linux

upx:
	strip --strip-unneeded $(name)_linux
	upx --lzma --best $(name)_linux

wasm:
	source ~/emsdk/emsdk_env.sh && emcc main.c -DWEB -O3 --closure 1 -s FILESYSTEM=0 -s ASYNCIFY -s USE_SDL=2 -s ENVIRONMENT=web -s TOTAL_MEMORY=64MB -o web/index.html --shell-file t.html && emrun web/index.html

js:
	source ~/emsdk/emsdk_env.sh && emcc main.c -DWEB -O3 -s MINIFY_HTML=0 --closure 0 -s WASM=0 -s FILESYSTEM=0 -s ASYNCIFY -s USE_SDL=2 -s ENVIRONMENT=web -s TOTAL_MEMORY=64MB -o web/index.html --shell-file t.html && emrun web/index.html

run:
	source ~/emsdk/emsdk_env.sh && emrun web/index.html

test:
	gcc main.c -I inc -Ofast -lSDL2 -lGLESv2 -lEGL -lm -o /tmp/$(name)_test
	/tmp/$(name)_test
	rm /tmp/$(name)_test

testsoft:
	gcc main.c -Ofast -lSDL2 -lGLESv2 -lEGL -lm -o /tmp/$(name)_test
	LIBGL_ALWAYS_SOFTWARE=1 /tmp/$(name)_test
	rm /tmp/$(name)_test

test60:
	gcc main.c -Ofast -lSDL2 -lGLESv2 -lEGL -lm -o /tmp/$(name)_test
	xrandr --rate 60 && /tmp/$(name)_test && xrandr --rate 160
	rm /tmp/$(name)_test

dbg:
	cc main.c -fsanitize=leak -fsanitize=undefined -fsanitize=address -ggdb3 -lSDL2 -lGLESv2 -lEGL -lm -o $(name)_dbg
	$(name)_dbg

grind:
	valgrind --track-origins=yes --leak-check=full --leak-check=full ./$(name)_linux

clean:
	rm -f $(name)_linux
	rm -f web/index.html
	rm -f web/index.js
	rm -f web/index.wasm
	rm -f dev/ptf