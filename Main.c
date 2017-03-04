#include "TinyShell.h"
#include <string.h>
#include <Windows.h>
#include <shlwapi.h>
int main ()
{
    char string[MAX_PATH], Command[MAX_SIZE_COMMAND], argumentCommandlines[MAX_PATH];
    ProcessTable prtable;
    ZeroMemory(&prtable, sizeof(prtable));
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
        strcpy(Command, argvCommand[0]);

        if(!isBatFile(Command))
            strcat(Command, ".bat");
        if(PathFileExists(Command)){
            exeBatFile(&prtable, Command, argcCommand, argvCommand, argumentCommandlines);
        }
        else {
            Command[strlen(Command) - 4] = '\0';
            exeCommand(&prtable, Command, argcCommand, argvCommand, argumentCommandlines);
        }

        deleteArgvCommand(argcCommand, argvCommand);
    } while (1);
    return 0;
}
