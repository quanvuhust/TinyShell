#include "Path.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
void Path()
{
    LPTSTR lpszVariable;
    LPTCH lpvEnv;
    // Get a pointer to the environment block.
    lpvEnv = GetEnvironmentStrings();
    // If the returned pointer is NULL, exit.
    if (lpvEnv == NULL){
        printf("GetEnvironmentStrings failed (%d)\n", GetLastError());
        return;
    }
    // Variable strings are separated by NULL byte, and the block is
    // terminated by a NULL byte.
    lpszVariable = (LPTSTR) lpvEnv;
    printf("\n");
    while (*lpszVariable){
        _tprintf(TEXT("%s\n"), lpszVariable);
        lpszVariable += lstrlen(lpszVariable) + 1;
    }
    FreeEnvironmentStrings(lpvEnv);
}

void addPath(int argc, char* argv[])
{
    if(argc == 1){
        printf("Too few arguments to addpath.\n");
        return;
    } else if(argc > 3){
        printf("Too many arguments to addpath.\n");
        return;
    }

    SetEnvironmentVariable(argv[1], argv[2]);
}
