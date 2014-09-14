PROGRAM = wallcolour
PROGRAM_FILES = wallcolour.cpp

CFLAGS += -O2 $(shell Magick++-config --cxxflags)
LIBS = -L/usr/local/lib
LIBS   += $(shell Magick++-config --libs)
EXTRALIBS += $(shell Wand-config --libs)
EXTRAFLAGS += -std=c++0x

all:
	g++ $(CFLAGS) $(LIBS) $(EXTRALIBS) $(EXTRAFLAGS) -o wallcolour $(PROGRAM_FILES)

install: all
