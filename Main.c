#include "TinyShell.h"
#include <string.h>
#include <Windows.h>
#include <shlwapi.h>
int main ()
{
    char string[MAX_PATH], argumentCommandlines[MAX_PATH];
    ProcessTable prtable;
    ZeroMemory(&prtable, sizeof(prtable));
    printf("%s\n", PathFindExtension("Test"));
    printf("Shell: Version [1.02.270217] \n");
    printf("(c) 2016 My Corporation. All rights reserved.\n\n");
    do {
        int argcCommand = 0;
        char **argvCommand = NULL;
        printCurrentDirectory();
        /*Nhap xau lenh tu man hinh, chuan hoa va tach cau lenh tu xau nhap vao*/
        inputString(string, sizeof(string));
        normalizedString(string);
        strcpy(argumentCommandlines, string);
        if(string[0] == '\0')
            continue;
        getArgc_Argv_For_Command(&argcCommand, &argvCommand, string);

        if(!isBatFile(argvCommand[0]))
            strcat(argvCommand[0], ".bat");
        if(PathFileExists(argvCommand[0])){
            exeBatFile(&prtable, argcCommand, argvCommand, argumentCommandlines);
        }
        else {
            argvCommand[0][strlen(argvCommand[0]) - 4] = '\0';
            exeCommand(&prtable, argcCommand, argvCommand, argumentCommandlines);
        }

        deleteArgvCommand(argcCommand, argvCommand);
    } while (1);
    return 0;
}
