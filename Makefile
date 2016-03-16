GL_INCLUDE = /usr/X11R6/include
GL_LIB = /usr/X11R6/lib

build: runner.c
	cc -o main.bin runner.c -Os -Wall -Wextra -Wpedantic -l5kgl -lglfw3 -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm

