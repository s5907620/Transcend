#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
  string diskLetters[4] = {"C", "D", "E", "F"};

  cout << "Disk Info:" << endl;
  const int GB = 1024 * 1024 * 1024;
  for (int i = 0; i < 4; ++i)
  {
    cout << "Partition [" << diskLetters[i] << "]" << endl;
    string diskPath = diskLetters[i] + ":\\";

    // all bytes
    ULARGE_INTEGER totalSize;
    ULARGE_INTEGER totalFreeSize;

    if (GetDiskFreeSpaceEx(diskPath.c_str(), nullptr, &totalSize, &totalFreeSize))
    {
      // to GB
      float totalSize_f = (float)totalSize.QuadPart / GB;
      float freeSize_f = (float)totalFreeSize.QuadPart / GB;
      cout << "Size Total (GB): " << totalSize_f << endl;
      cout << "Size Used (GB): " << freeSize_f << endl;
    }
    else
      throw logic_error("Failed to get disk space info");
    cout << endl;
  }
}