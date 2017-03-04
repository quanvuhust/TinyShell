#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#define ZERO '0'
void longlongToChar(long long Number, char *Destination)
{
    int i = 0, j = 0;
    char numberChar[30];
    while(Number != 0){
        numberChar[i++] = (Number%10) + ZERO;
        Number /= 10;
    }
    numberChar[i] = '\0';
    for(i = strlen(numberChar) - 1; i >= 0; i--)
        Destination[j++] = numberChar[i];
    Destination[j] = '\0';
}
void addCommaToLong(long long Number, char *Destination)
{
    int i, len, j, countNumber = 0;
    char numberChar[30];
    longlongToChar(Number, numberChar);
    len = strlen(numberChar);
    for(i = len - 1; i >= 0; i--){
            countNumber++;
            if(countNumber == 3 && i != 0){
                for(j = len; j >= i; j--)
                    numberChar[j + 1] = numberChar[j];
                numberChar[i] = ',';
                len += 1;
                countNumber = 0;
            }
    }
    strcpy(Destination, numberChar);
}

void dir(int argc, char *argv[])
{
    if (argc > 1){
        printf("Too many arguments to command dir.\n");
        return;
    }
    int countFile = 0, countDir = 0;
    char numberChar[30];
    long long sumSizeFile = 0;
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR curDirectoryPath[MAX_PATH];
    TCHAR szDir[MAX_PATH];
    SYSTEMTIME sysTime;
    GetCurrentDirectory(MAX_PATH, curDirectoryPath);
    strcpy(szDir, curDirectoryPath);
    printf("\n Directory of %s\n\n", szDir);
    strcat(szDir, "\\*");
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFile(szDir, &ffd);

    do {
        FileTimeToSystemTime(&ffd.ftCreationTime, &sysTime);
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            countDir++;
            _tprintf(TEXT("%02u/%02u/%04u  %02u:%02u    <DIR>%-10s %s\n"),sysTime.wDay, sysTime.wMonth,
                                                                          sysTime.wYear, sysTime.wHour, sysTime.wMinute,
                                                                          " ", ffd.cFileName);
        } else {
            countFile++;
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            sumSizeFile += filesize.QuadPart;
            addCommaToLong(filesize.QuadPart, numberChar);
            _tprintf(TEXT("%02u/%02u/%04u  %02u:%02u         %-10s %s\n"),sysTime.wDay, sysTime.wMonth, sysTime.wYear,
                                                                            sysTime.wHour, sysTime.wMinute, numberChar,
                                                                            ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    addCommaToLong(sumSizeFile, numberChar);
    printf("%14d File(s) %14s bytes\n", countFile, numberChar);
    printf("%14d Dir(s)\n", countDir);
    FindClose(hFind);
}
