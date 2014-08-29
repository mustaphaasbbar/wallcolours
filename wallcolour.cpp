#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iomanip>
#include <Magick++.h>
using namespace std;
using namespace Magick;


class ImageRGB {
  //string image_filename;
public:
  ImageRGB(string);
  unsigned long red = 0, green = 0, blue = 0;
};

ImageRGB::ImageRGB(string image_filename)
{
  Image image(image_filename);
  unsigned long counter = 0;
  
  vector<pair<Color, unsigned long>> histogram;
  colorHistogram(&histogram, image);
  vector<pair<Color, unsigned long>>::const_iterator p = histogram.begin();
  
  while (p != histogram.end())
  {
    red   = red + (int)p->first.redQuantum();
    green = green + (int)p->first.greenQuantum();
    blue  = blue + (int)p->first.blueQuantum();
    p++;
    counter++;
  }
  
  red   = red / counter * 255 / 65535;
  green = green / counter * 255 / 65535;
  blue  = blue / counter * 255 / 65535;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cout << "You need to give me a filename to work with." << endl;
    return 1;
  }
  
  ifstream given(argv[1]);
  
  if (!given)
  {
    cout << "Unable to open that filename." << endl;
    return 1;
  }
  
  ImageRGB wallpaper(argv[1]);
  cout << wallpaper.red << ", " << wallpaper.green << ", " << wallpaper.blue << endl;
  return 0;
}