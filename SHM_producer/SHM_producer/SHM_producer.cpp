// SHM_producer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <string>

#define BUF_SIZE 512
char szName[256] = "Global\\MyFileMappingObject";
char szName1[256] = "Global\\MyFileMappingObject1";
int _tmain(int argc, _TCHAR* argv[])
{
	  HANDLE hMapFile;
	  HANDLE hEvent;

   LPCTSTR pBuf;

   hEvent = CreateEvent(NULL,true,false,(LPCWSTR)szName1);
   

   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   (LPWSTR)szName);               // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }

   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

      CloseHandle(hMapFile);

      return 1;
   }

   std::cout << "Hi! I am SHM Consumer will display the messages recived on /""Global\\MyFileMappingObject1/""\n\n";

   char *output = new char[BUF_SIZE];
   
   int count = 0;

   while(1)
   {
	   WaitForSingleObject(hEvent,INFINITE);
	   ResetEvent(hEvent);

	   CopyMemory(output,pBuf,BUF_SIZE);

	   std::cout << "Message_" << count++ << ":";
	   std::cout << output <<std::endl;
	   
	   
	   ZeroMemory(output,BUF_SIZE);
   }


   UnmapViewOfFile(pBuf);

   CloseHandle(hMapFile);

	return 0;
}

