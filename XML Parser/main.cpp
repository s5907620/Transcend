#include <iostream>
#include "xml.h"

using namespace std;

int main()
{
  xml::Xml root("init");
  if (root.load("./readFile.xml"))
    cout << root.outputStr() << endl;
  else
    cout << "Failed to open file" << endl;
}