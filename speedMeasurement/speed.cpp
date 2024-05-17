// compile command g++ -g speed.cpp -lwininet -o a.exe
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <chrono>

#pragma comment(lib, "wininet.lib")

using namespace std;

void showProgress(double progress);

int main()
{
  // dowmload steps: InternetOpen -> InternetOpenUrl ->
  //                 InternetReadFile -> InternetCloseHandle
  // init an Internet session
  HINTERNET myInternet = InternetOpen("MyApp", INTERNET_OPEN_TYPE_DIRECT,
                                      NULL, NULL, 0);

  if (myInternet != nullptr)
  {
    // target url
    const string url = "https://s3-ap-northeast-1.amazonaws.com/test.storejetcloud.com/candidate_dummyFile.bin";
    // open target url
    // INTERNET_FLAG_RELOAD 強制重讀(都載最新版)
    HINTERNET myUrl = InternetOpenUrl(myInternet, url.c_str(), nullptr,
                                      0, INTERNET_FLAG_RELOAD, 0);
    if (myUrl != nullptr)
    {
      // get file size in bytes
      DWORD fileSize = 1073741824; // 1 GB to bytes

      // reading file
      char buffer[1000000];
      DWORD bytesRead;
      double hasReadBytes = 0;

      // time
      auto start_t = chrono::steady_clock::now();
      while (InternetReadFile(myUrl, buffer, sizeof(buffer), &bytesRead) &&
             bytesRead > 0)
      {
        hasReadBytes += bytesRead;
        double progress = hasReadBytes / fileSize;
        showProgress(progress);

        // write speed
        auto cur_t = chrono::steady_clock::now();
        auto elapsed_t = chrono::duration_cast<chrono::milliseconds>(cur_t - start_t).count() / 1000.0; // to s
        double speed = hasReadBytes / (1024 * 1024 * elapsed_t);                                        // MB/s

        cout << "Write Speed: " << speed << " MB/s  " << "\r" << endl;
      }
      // close url
      InternetCloseHandle(myUrl);
    }
    else
      throw logic_error("Failed to open url!!!");

    // close Internet session
    InternetCloseHandle(myInternet);
  }
  else
    throw logic_error("Failed to init an Internet session!!!");
}

void showProgress(double progress)
{
  const int bar = 50;
  cout << "[";
  int pos = (int)(bar * progress);
  for (int i = 0; i < bar; ++i)
  {
    if (i < pos)
      cout << "=";
    else if (i == pos)
      cout << ">";
    else
      cout << " ";
  }
  cout << "] " << (int)(progress * 100.0) << " %\r" << endl;
}