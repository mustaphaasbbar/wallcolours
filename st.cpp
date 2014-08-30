#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;


class SchemeFile {
public:
  SchemeFile(char *);
  void SchemeReplace();
  string baseContents;
};

SchemeFile::SchemeFile(char * filename)
{
  cout << "in constructor" << endl;
  
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
  

void SchemeFile::SchemeReplace()
{
  cout << "in replace" << endl;
  string replacement("123,234,345");
  baseContents.replace(baseContents.find("xxx,xxx,xxx"), replacement.length(), replacement);
  cout << baseContents << endl;
}


int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cout << "Give me something to work with here!" << endl;
    exit(EXIT_FAILURE);
  }

  SchemeFile newone(argv[1]);
  newone.SchemeReplace();
  
  return 0;
}
