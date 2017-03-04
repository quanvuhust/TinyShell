#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shlwapi.h>
#include "TinyShell.h"
#include "DateTime.h"
#include "Directory.h"
#include "Process.h"
#include "Path.h"
#include <string.h>
#include <ctype.h>
#define NUMBER_KIND_COMMANDS 5
#define SPACE 32

const char* SHELL_COMMAND[NUMBER_SHELL_COMMANDS] = {"exit", "help"};
enum COMMAND {EXIT, HELP};
const char *DATETIME_COMMANDS[NUMBER_DATETIME_COMMANDS] = {"date", "time"};
enum DATETIME {DATES, TIMES};
const char *PROCESS_COMMANDS[NUMBER_PROCESS_COMMANDS] = {"kill", "list", "resume", "stop"};
enum PROCESS {KILL, LIST, RESUME, STOP};
const char *DIRECTORY_COMMANDS[NUMBER_DIRECTORY_COMMANDS] = {"dir"};
enum DIRECTORY {DIR};
const char *ENVIRONMENT_COMMANDS[NUMBER_ENVIRONMENT_COMMANDS] = {"addpath", "path"};
enum ENVIRONMENT {ADDPATH, PATH};

const char **COMMANDS[NUMBER_KIND_COMMANDS]      = {DATETIME_COMMANDS, DIRECTORY_COMMANDS, ENVIRONMENT_COMMANDS,
                                                    PROCESS_COMMANDS, SHELL_COMMAND
                                                   };

const int sizeKindCommands[NUMBER_KIND_COMMANDS] = {NUMBER_DATETIME_COMMANDS, NUMBER_DIRECTORY_COMMANDS, NUMBER_ENVIRONMENT_COMMANDS,
                                                    NUMBER_PROCESS_COMMANDS, NUMBER_SHELL_COMMANDS
                                                   };

enum KIND_COMMANDS                                 {KIND_DATETIME_COMMANDS, KIND_DIRECTORY_COMMANDS, KIND_ENVIRONMENT_COMMANDS,
        KIND_PROCESS_COMMANDS, KIND_SHELL_COMMANDS
                                                   };

void CleanBuffer (void)
{
    fflush (stdin);
}
void ClearScreen (void)
{
    system("cls");
}
int exitShell(int argc)
{
    if (argc > 1) {
        printf("Too many arguments to command exit.\n");
        return 1;
    }
    exit (0);
}

int MallocString (char **s, unsigned Number)
{
    *s = malloc (Number * sizeof(char));
    return (*s == NULL);
}

void inputString (char *str, unsigned MaxSize)
{
    CleanBuffer ();
    fgets (str, MaxSize + 1, stdin);
    str[strlen(str) - 1] = '\0';
}

int BinarySearch (const char *a[], unsigned size, char *s)
{
    int lower = 0, upper = size - 1;
    unsigned Mid;

    while (lower <= upper) {
        Mid = (lower + upper)/2;
        if (strcmp(s, *(a + Mid)) > 0)
            lower = Mid + 1;
        else if (strcmp(s, *(a + Mid)) < 0)
            upper = Mid -1;
        else
            return Mid;
    }

    return -1;
}

void printCurrentDirectory(void)
{
    char curDirectoryPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curDirectoryPath);
    printf("%s>", curDirectoryPath);
}

void helpShell(int argc)
{
    if (argc > 1) {
        printf("Too many arguments to command help.\n");
        return;
    }

    puts("\t\tWELCOME TO MY SHELL\n");
    puts("\tMy Shell supports the following commands:");
    /*DIRECTORY COMMANDS*/
    puts("DIRECTORY");
    printf("\t%-8s : List the content of the current directory.\n", "dir");
    /*PROCESS COMMANDS*/
    puts("PROCESS");
    printf("\t%-8s : List of process.\n", "list");
    printf("\t%-8s : Kill a process.\n", "kill");
    printf("\t%-8s : Stop a process is running.\n", "stop");
    printf("\t%-8s : Resume a process is stopping.\n", "resume");
    /*DATE TIME COMMANDS*/
    puts("DATE TIME");
    printf("\t%-8s : Displays or sets the date.\n", "date");
    printf("\t%-8s : Displays or sets the system time.\n", "time");
    /*ENVIRONMENT VARIABLE COMMANDS*/
    puts("ENVIRONMENT VARIABLE");
    printf("\t%-8s : Displays or sets a search path for executable files.\n", "path");
    printf("\t%-8s : Add a path to environment variable.\n", "addpath");
    /*SHELL COMMANDS*/
    puts("SHELL");
    printf("\t%-8s : Provides Help information for Shell commands.\n", "help");
    printf("\t%-8s : Quits the TinyShell.EXE program (command interpreter).\n", "exit");
}

void deleteCharacter(char *str, int position, int len)
{
    int i;
    for(i = position; i < len; i++)
        str[i] = str[i + 1];
}

void normalizedString(char *str)
{
    int i, len = strlen(str);
    char *ptr = str;

    /*Xoa khoang trong cuoi xau*/
    for(i = len - 1; isspace(str[i]); i--);
    str[++i] = '\0';
    len = i;
    /*Xoa khoang trong dau xau*/
    for(i = 0; isspace(str[i]); i++)
        ptr++;
    strcpy(str, ptr);
    len -= i;
    /*Xoa khoang trong giua xau*/
    for(i = 1; i < len - 1; i++)
        if(isspace(str[i])) {
            str[i] = SPACE;
            i++;
            while(isspace(str[i])) {
                deleteCharacter(str, i, len);
                len--;
            }
        }
}

void getArgc_Argv_For_Command(int *argc_ptr, char ***argv_ptr, char *Source)
{
    int i, countSpace = 0, len = strlen(Source);
    for(i = 0; i < len; i++)
        if(Source[i] == SPACE)
            countSpace++;
    *argc_ptr = countSpace + 1;
    *argv_ptr = (char **) malloc(*argc_ptr*sizeof(char*));

    int j = *argc_ptr - 1;
    for(i = len - 1; i >= 0; i--) {
        if(i == 0) {
            (*argv_ptr)[0] = (char *)malloc((strlen(Source) + 1)*sizeof(char));
            strcpy((*argv_ptr)[0], Source);
        }

        if(Source[i] == SPACE) {
            (*argv_ptr)[j] = (char *)malloc((strlen(Source) - i)*sizeof(char));
            strcpy((*argv_ptr)[j--], (Source + i + 1));
            Source[i] = '\0';
        }
    }
}

void deleteArgvCommand(int argcCommand, char **argvCommand)
{
    int i;
    for(i = 0; i < argcCommand; i++)
        free(argvCommand[i]);
    free(argvCommand);
}

int isExeFile(char *Command)
{
    int i;
    for(i = strlen(Command) - 1; i >= 0; i--)
        if(Command[i] == '.')
            break;
    if(i == 0)
        return 0;
    else if(!strcmp(Command + i, ".exe"))
        return 1;
    else
        return 0;
}

int isBatFile(char *Command)
{
    int i;
    for(i = strlen(Command) - 1; i >= 0; i--)
        if(Command[i] == '.')
            break;
    if(i == 0)
        return 0;
    else if(!strcmp(Command + i, ".bat"))
        return 1;
    else
        return 0;
}

void exeCommand(ProcessTable *prtable_ptr, char *Command, int argcCommand, char *argvCommand[], char *argumentCommandlines)
{
    int commandIndex, i;

    /*Tim kiem cau lenh nhap vao co ton tai hay khong*/
    for(i = 0; i < NUMBER_KIND_COMMANDS; i++) {
        commandIndex = BinarySearch (COMMANDS[i], sizeKindCommands[i], Command);
        if(commandIndex != -1)
            break;
    }
    if(commandIndex != -1) {
        switch(i) {
        case KIND_DATETIME_COMMANDS: {
            switch(commandIndex) {
            case DATES:
                dateCommand(argcCommand);
                break;
            case TIMES:
                timeCommand(argcCommand);
                break;
            }
            break;
        }
        case KIND_DIRECTORY_COMMANDS: {
            switch(commandIndex) {
            case DIR:
                dir(argcCommand, argvCommand);
                break;
            }
            break;
        }
        case KIND_ENVIRONMENT_COMMANDS: {
            switch(commandIndex) {
            case PATH:
                Path();
                break;

            case ADDPATH:
                addPath(argcCommand, argvCommand);
                break;
            }
            break;
        }
        case KIND_PROCESS_COMMANDS: {
            switch(commandIndex) {
            case KILL:
                killProcess(argcCommand, argvCommand, prtable_ptr);//Truyen argvCommand vao khong truyen atoi
                break;

            case LIST:
                listProcess(argcCommand, *prtable_ptr);
                break;
            case RESUME:
                resumeProcess(argcCommand, argvCommand, *prtable_ptr);
                break;
            case STOP:
                stopProcess(argcCommand, argvCommand, *prtable_ptr);
                break;
            }
            break;
        }
        case KIND_SHELL_COMMANDS: {
            switch(commandIndex) {
            case HELP:
                helpShell(argcCommand);
                break;
            case EXIT:
                exitShell(argcCommand);
                break;
            }
            break;
        }
        }
    } else {
        if(!isExeFile(Command))
            strcat(Command, ".exe");
        if(PathFileExists(Command)) {
            if(!strcmp(argvCommand[argcCommand - 1], "&")) {
                argumentCommandlines[strlen(argumentCommandlines) - 2] = '\0';
                prtable_ptr->Table[prtable_ptr->NumberProcess++] = createProcess(Command, argumentCommandlines, &modeBackground);
            } else {
                createProcess(Command, argumentCommandlines, &modeForeground);
                printf("\n");
            }
        } else {
            Command[strlen(Command) - 4] = '\0';
            printf("\'%s\' is not recognized as an internal or external command, operable program or batch file.\n", Command);
        }
    }
}

void exeBatFile(ProcessTable *prtable_ptr, char *Command, int argcCommand, char *argvCommand[], char *argumentCommandlines)
{
    FILE *f;
    f = fopen(Command, "rt");
    if(f == NULL) {
        printf("Can't execute %s.\n", Command);
    } else {
        char commandOfBat[MAX_PATH];
        while(!feof(f)) {
            fgets(commandOfBat, MAX_PATH + 1, f);
            commandOfBat[strlen(commandOfBat) - 1] = '\0';
            printCurrentDirectory();
            printf("%s\n", commandOfBat);
            normalizedString(commandOfBat);
            strcpy(argumentCommandlines, commandOfBat);
            if(commandOfBat[0] == '\0')
                continue;
            getArgc_Argv_For_Command(&argcCommand, &argvCommand, commandOfBat);
            strcpy(Command, argvCommand[0]);
            exeCommand(prtable_ptr, Command, argcCommand, argvCommand, argumentCommandlines);
        }
        fclose(f);
    }
}
