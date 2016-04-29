build: src/5kgl.h src/*.c
	mkdir -p include bin lib
	cp src/5kgl.h include/5kgl.h
	cd bin/; \
	gcc -fpic -Os -Wall -Werror -Wpedantic -Wextra -c ../src/*.c ../src/5kgl.h ;\
	gcc -shared -fpic -Wall -Werror -Wpedantic -Wextra -o ../lib/lib5kgl.so *.o -lc

install: lib/lib5kgl.so include/5kgl.h
	sudo cp lib/lib5kgl.so /usr/lib/
	sudo cp include/5kgl.h /usr/local/include/

docs: src/* Doxyfile
	doxygen Doxyfile

demo: demo.c
	cc -o demo.bin demo.c -O3 -Wall -Werror -Wextra -Wpedantic -l5kgl -lglfw -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm
