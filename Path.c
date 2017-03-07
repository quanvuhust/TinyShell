#include "Path.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>
void Path()
{
    LPTSTR lpszVariable;
    LPTCH lpvEnv;
    lpvEnv = GetEnvironmentStrings();
    if (lpvEnv == NULL){
        printf("GetEnvironmentStrings failed (%d)\n", GetLastError());
        return;
    }

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
