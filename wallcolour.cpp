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

/* 0.0 is monochrome
 * 1.0 is neutral (avg from the image)
 * over 1.0 increases saturation
 */


#define SATURATION_MULTIPLIER 1.0

using namespace std;
using namespace Magick;


class ImageRGB
{
public:
    ImageRGB(string);
    unsigned long red = 0, green = 0, blue = 0;
};

ImageRGB::ImageRGB(string image_filename)
{
    // Image image(image_filename);
	Image image;
	image.read(image_filename);
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


class SchemeFile
{
public:
    SchemeFile(string);
    string SchemeReplace(string);
    string baseContents;
};

SchemeFile::SchemeFile(string filename)
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



class beShellConfig
{
    string contents;
public:
    beShellConfig();
    string showContents();
    string getWallpaper();
};


beShellConfig::beShellConfig()
{
    string home = getenv("HOME");
    string kdeLocalPrefix = home + "/.kde4";
    struct stat sb;

    stat(kdeLocalPrefix.c_str(), &sb);

    if (!S_ISDIR(sb.st_mode)) {
        kdeLocalPrefix = home + "/.kde";
        stat(kdeLocalPrefix.c_str(), &sb);

        if (!S_ISDIR(sb.st_mode)) {
            cout << "unable to locate kde local directory." << endl;
        }
    }

    string beshellConfigFile = kdeLocalPrefix + "/share/config/be.shell";

    ifstream shellConfig(beshellConfigFile);

    if (shellConfig.is_open()) {
        stringstream strStream;
        strStream << shellConfig.rdbuf();
        contents = strStream.str();
        shellConfig.close();
    } else {
        cout << "Unable to open template file." << endl;
        exit(EXIT_FAILURE);
    }
}

string beShellConfig::showContents()
{
    return contents;
}

string beShellConfig::getWallpaper()
{
    string::iterator it = contents.begin();
    string keyValue;
    size_t found = contents.find("Wallpaper=");

    if (found != string::npos) {
        it = it + found + 10;

        for(it; it != contents.end(); it++) {
            if (*(it) == '\n')
                break;
            keyValue += *(it);
        }
    } else {
        cout << "wallpaper name not found" << endl;
        exit(EXIT_FAILURE);
    }

    return keyValue;
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
    beShellConfig myShellConfig;
    string myWall = myShellConfig.getWallpaper();

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
    
    string colTemplate = getenv("HOME");
    colTemplate += "/.config/wallcolour/colorscheme_template";
    SchemeFile schemeTemplate(colTemplate);
    string newScheme = schemeTemplate.SchemeReplace(replacementString);
    writeScheme(newScheme);
    return 0;
}
