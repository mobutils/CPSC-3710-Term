INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/lib/

COMPILERFLAGS = -Wall
CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)

#LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
#lxi and lXmu were removed since on local system they are not required
#and in fact cause build errors.

LIBRARIES = -lX11 -lglut -lGL -lGLU -lm

All: Robot

Robot: RobotGame.o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)
