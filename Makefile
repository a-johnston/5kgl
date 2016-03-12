GL_INCLUDE = /usr/X11R6/include
GL_LIB = /usr/X11R6/lib

build: runner.c render.h mesh.h color.h space_math.h util.h physics.h
	gcc -o main.bin runner.c -Os -Wall -Wextra -Wpedantic -L$(GL_LIB) -lglfw3 -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm

test: test.c render.h mesh.h color.h space_math.h util.h physics.h
	gcc -o test.bin test.c -Os -Wall -Wextra -Wpedantic -L$(GL_LIB) -lglfw3 -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm

