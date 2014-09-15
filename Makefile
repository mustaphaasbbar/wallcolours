PROGRAM = wallcolour
PROGRAM_FILES = wallcolour.cpp

CFLAGS = -O2 $(shell pkg-config --cflags ImageMagick++)
LIBS   = $(shell pkg-config --libs ImageMagick++)
EXTRAFLAGS = -std=c++0x

all:
	g++ $(CFLAGS) $(LIBS)  $(EXTRAFLAGS) -o wallcolour $(PROGRAM_FILES)

install: all
