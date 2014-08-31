#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;


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
  
  cout<< baseContents << endl;
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
    cout << "Give me something to work with here!" << endl;
    exit(EXIT_FAILURE);
  }
  
  string schemecolour("123,254,109");
  SchemeFile newone(argv[1]);
  newone.SchemeReplace(schemecolour);
  
  return 0;
}
