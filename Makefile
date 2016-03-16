GL_INCLUDE = /usr/X11R6/include
GL_LIB = /usr/X11R6/lib

build: demo.c
	cc -o demo.bin demo.c -Os -Wall -Wextra -Wpedantic -l5kgl -lglfw3 -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm

