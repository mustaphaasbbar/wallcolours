A simple tool so set system colours from the wallpaper image. 
To compile:
	g++ -std=c++0x  $(pkg-config --cflags --libs ImageMagick++) -O2 wallcolour.cpp -o wallcolour

To run:
	./wallcolour [/path/to/image] > myname.colors

then copy myname.colors to .kde/share/apps/color-schemes
