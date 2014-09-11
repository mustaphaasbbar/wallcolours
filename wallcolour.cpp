#include <iostream>
#include <fstream>
#include <sstream>
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

   vector < pair < Color, unsigned long >> histogram;
   colorHistogram(&histogram, image);
   vector < pair < Color, unsigned long >> ::const_iterator p = histogram.begin();

   while (p != histogram.end())
   {
      red   = red + static_cast<int>(p->first.redQuantum());
      green = green + static_cast<int>(p->first.greenQuantum());
      blue  = blue + static_cast<int>(p->first.blueQuantum());
      p++;
      counter++;
   }

   red   = red / counter * 255 / 65535;
   green = green / counter * 255 / 65535;
   blue  = blue / counter * 255 / 65535;
}


class SchemeFile {
public:
  SchemeFile(char *);
  void SchemeReplace(string);
  string baseContents;
};

SchemeFile::SchemeFile(char * filename)
{
  ifstream baseTemplate (filename);
  
  if (baseTemplate.is_open())
  {
    stringstream strStream;
    strStream << baseTemplate.rdbuf();
    baseContents = strStream.str();
    baseTemplate.close();
  }
  else
  {
    cout << "Unable to open template file." << endl;
    exit(EXIT_FAILURE);
  }
}
  

void SchemeFile::SchemeReplace(string rgb_triplet)
{
  baseContents.replace(baseContents.find("xxx,xxx,xxx"), rgb_triplet.length(), rgb_triplet);
  cout << baseContents << endl;
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
   
   string replacementString = to_string(wallpaper.red) + "," + to_string(wallpaper.green) + \
		"," + to_string(wallpaper.blue);
   //cout << replacementString << endl;
   
   while (1) 
   {
		if (replacementString.length() < 11)
			replacementString = replacementString + " ";
		else
			break;
   }
   
   //cout << replacementString.length() << endl;
   
   SchemeFile schemeTemplate((char *)"colorscheme_template");
   schemeTemplate.SchemeReplace(replacementString);
   
   return 0;
}
