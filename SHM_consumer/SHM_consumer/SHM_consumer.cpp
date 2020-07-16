#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

#define BUF_SIZE 512
char szName[256] = "Global\\MyFileMappingObject";
char szName1[256] = "Global\\MyFileMappingObject1";
//TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[]=TEXT("Message from first process.");

int _tmain()
{
   HANDLE hMapFile;
   LPCTSTR pBuf;

   	  HANDLE hEvent;


   


   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 BUF_SIZE,                // maximum object size (low-order DWORD)
                 (LPCWSTR)szName);        // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }
   pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
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

   hEvent = CreateEvent(NULL,true,false,(LPCWSTR)szName1);

   std::cout << "Hi! I am SHM Producer will send the messages to other client\n\n";

   while(1)
   {
	   char msg[256] ={0};
	   std::cout << "Enter the message for client\n";
	   std::cin >> msg;
	   CopyMemory((PVOID)pBuf, msg, (_tcslen(szMsg) * sizeof(TCHAR)));
	   SetEvent(hEvent);

   }

   UnmapViewOfFile(pBuf);

   CloseHandle(hMapFile);

   return 0;
}