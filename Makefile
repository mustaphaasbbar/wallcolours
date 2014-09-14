PROGRAM = wallcolour
PROGRAM_FILES = wallcolour.cpp

CFLAGS += -O2 $(shell Magick++-config --cxxflags)
LIBS   += $(shell Magick++-config --libs)
EXTRAFLAGS += -std=c++0x

all:
	g++ $(CFLAGS) $(LIBS) $(EXTRAFLAGS) -o wallcolour $(PROGRAM_FILES)

install: all
