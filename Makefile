PROGRAM = wallcolour
PROGRAM_FILES = wallcolour.cpp

#CFLAGS = -O2 $(shell pkg-config --cflags ImageMagick++)
#LIBS   = $(shell pkg-config --libs ImageMagick++)
#EXTRAFLAGS = -std=c++0x -Os
#MAGICKFLAGS=$(shell Magick++-config --cxxflags --libs) 

#I have no idea why this line needs to be written like this
all:
	g++ $(shell Magick++-config --cxxflags --cppflags) -Os -std=c++0x  -o wallcolour $(PROGRAM_FILES) $(shell Magick++-config --ldflags --libs)

install: all
	sudo install wallcolour /usr/bin 
	mkdir -pv $(HOME)/.config/wallcolour
	install -m 644 colorscheme_template  $(HOME)/.config/wallcolour

