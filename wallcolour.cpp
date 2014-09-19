#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <Magick++.h>

#define  Pr  .299
#define  Pg  .587
#define  Pb  .114

#define SATURATION_MULTIPLIER 2.0

using namespace std;
using namespace Magick;


class ImageRGB {
  public:
    ImageRGB(string);
    unsigned long red = 0, green = 0, blue = 0;
};

ImageRGB::ImageRGB(string image_filename)
{
    Image image(image_filename);
    unsigned long counter = 0;

    vector < pair < Color, unsigned long >>histogram;
    colorHistogram(&histogram, image);
    vector < pair < Color, unsigned long >>::const_iterator p =
        histogram.begin();

    while (p != histogram.end()) {
        red = red + static_cast < int >(p->first.redQuantum());
        green = green + static_cast < int >(p->first.greenQuantum());
        blue = blue + static_cast < int >(p->first.blueQuantum());
        p++;
        counter++;
    }

    red = red / counter * 255 / 65535;
    green = green / counter * 255 / 65535;
    blue = blue / counter * 255 / 65535;
}


class SchemeFile {
  public:
    SchemeFile(char *);
    string SchemeReplace(string);
    string baseContents;
};

SchemeFile::SchemeFile(char *filename)
{
    ifstream baseTemplate(filename);

    if (baseTemplate.is_open()) {
        stringstream strStream;
        strStream << baseTemplate.rdbuf();
        baseContents = strStream.str();
        baseTemplate.close();
    } else {
        cout << "Unable to open template file." << endl;
        exit(EXIT_FAILURE);
    }
}


string SchemeFile::SchemeReplace(string rgbTriplet)
{
    baseContents.replace(baseContents.find("xxx,xxx,xxx"),
                         rgbTriplet.length(), rgbTriplet);
    return baseContents;
}

string getWallpaper()
{
    string home = getenv("HOME");
    string beshellConfigFile = home + "/.kde4/share/config/be.shell";
    struct stat sb;

    stat(beshellConfigFile.c_str(), &sb);

    if (!S_ISREG(sb.st_mode)) {
        beshellConfigFile = home + "/.kde/share/config/be.shell";
        stat(beshellConfigFile.c_str(), &sb);

        if (!S_ISREG(sb.st_mode)) {
            cout << "really can't find it" << endl;
            exit(EXIT_FAILURE);
        }
    }

    ifstream beConfig(beshellConfigFile);

    if (beConfig.is_open()) {
        string aline;
        size_t found;
        while (beConfig.good()) {
            getline(beConfig, aline);
            found = aline.find("Wallpaper=");
            if (found != string::npos) {
                string wallpaperFilename = aline.substr(10);
                return wallpaperFilename;
            }
        }
        beConfig.close();
    } else {
        cout << "failed opening config file" << endl;
        exit(EXIT_FAILURE);
    }
}

void writeScheme(string contents)
{
    string home = getenv("HOME");
    string kdeColorsDir = home + "/.kde4/share/apps/color-schemes";
    struct stat sb;

    stat(kdeColorsDir.c_str(), &sb);
    if (!S_ISDIR(sb.st_mode)) {
        kdeColorsDir = home + "/.kde/share/apps/color-schemes";
        stat(kdeColorsDir.c_str(), &sb);

        if (!S_ISDIR(sb.st_mode)) {
            cout << " i really have no idea where to write to" << endl;
            exit(EXIT_FAILURE);
        }
    }

    string restingPlace = kdeColorsDir + "/Wallpaper.colors";
    ofstream ofile(restingPlace, ios::trunc);

    if (ofile.is_open()) {
        ofile << contents;
        ofile.close();
    } else {
        cout << "can't write output" << endl;
    }
}

//  public-domain function by Darel Rex Finley
void changeSaturation(float *R, float *G, float *B, float change)
{
    float  P=sqrt(
    (*R)*(*R)*Pr+
    (*G)*(*G)*Pg+
    (*B)*(*B)*Pb ) ;
    
    *R=P+((*R)-P)*change;
    *G=P+((*G)-P)*change;
    *B=P+((*B)-P)*change;
}



int main(int argc, char **argv)
{
    string myWall = getWallpaper();
    ImageRGB wallpaper(myWall);
    float fRed = (float)wallpaper.red;
    float fGreen = (float)wallpaper.green;
    float fBlue = (float)wallpaper.blue;

    changeSaturation(&fRed, &fGreen, &fBlue, SATURATION_MULTIPLIER);
    
    if (fRed > 255.0)
        fRed = 255.0;
    if (fGreen > 255.0)
        fGreen = 255.0;
    if (fBlue > 255.0)
        fBlue = 255.0;
        
    
    string replacementString =
        to_string((int)fRed) + "," + to_string((int)fGreen) + "," +
        to_string((int)fBlue);

    while (1) {
        if (replacementString.length() < 11)
            replacementString = replacementString + " ";
        else
            break;
    }

    SchemeFile schemeTemplate((char *) "colorscheme_template");
    string newScheme = schemeTemplate.SchemeReplace(replacementString);
    writeScheme(newScheme);
    return 0;
}
