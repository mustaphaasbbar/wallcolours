PROGRAM = wallcolour
PROGRAM_FILES = wallcolour.cpp

#CFLAGS = -O2 $(shell pkg-config --cflags ImageMagick++)
#LIBS   = $(shell pkg-config --libs ImageMagick++)
EXTRAFLAGS = -std=c++0x -Os
MAGICKFLAGS=$(shell Magick++-config --cxxflags --libs) 

all:
	g++ $(MAGICKFLAGS)  $(EXTRAFLAGS) -o wallcolour $(PROGRAM_FILES)

install: all
	install wallcolour $(HOME)/bin 
	mkdir -pv $(HOME)/.config/wallcolour
	install -m 644 colorscheme_template  $(HOME)/.config/wallcolour

