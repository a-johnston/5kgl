GL_INCLUDE = /usr/X11R6/include
GL_LIB = /usr/X11R6/lib

build: runner.c 5kgl.h src/*
	cc -o main.bin runner.c -Os -Wall -Wextra -Wpedantic -L$(GL_LIB) -lglfw3 -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm -I "src/"

