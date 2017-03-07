#include <stdio.h>
#include <string.h>
#include "Process.h"
#ifndef TINY_SHELL_H
#define TINY_SHELL_H
#define MAX_SIZE_COMMAND 35
#define NUMBER_SHELL_COMMANDS 2

int exitShell(int argc);
void CleanBuffer (void);
void ClearScreen (void);
int BinarySearch (const char *a[], unsigned size, char *s);
BOOL exeExists(char *filename);
void printCurrentDirectory(void);
void helpShell(int argc);
void inputString (char *str, unsigned MaxSize);
void deleteCharacter(char *str, int position, int len);
void normalizedString(char *str);
void getArgc_Argv_For_Command(int *argc_ptr, char ***argv_ptr, char *Source);
void deleteArgvCommand(int argcCommand, char **argvCommand);
int isBatFile(char *Command);
void exeCommand(ProcessTable *prtable_ptr, int argcCommand, char *argvCommand[], char *argumentCommandlines);
void exeBatFile(ProcessTable *prtable_ptr, int argcCommand, char *argvCommand[], char *argumentCommandlines);
#endif // TINY_SHELL_H
