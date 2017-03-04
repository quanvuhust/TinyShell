#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#define MAX_NAME 251
#define MAX_STATUS 9
#define MAX_PROCESS_NUMBER 36
#define NUMBER_PROCESS_COMMANDS 4

typedef struct ProcessTable
{
    PROCESS_INFORMATION Table[MAX_PROCESS_NUMBER];
    DWORD NumberProcess;
}ProcessTable;

int modeBackground(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram,
                   char* commandlineArguments);

int modeForeground(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram,
                   char* commandlineArguments);

PROCESS_INFORMATION createProcess(const char* nameProgram, char* commandlineArguments,
                                  int (*modeProcess)(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram, char* commandlineArguments));

int checkStatusThread(HANDLE hThread);
void deleteElement(PROCESS_INFORMATION table[], int position, int len);
void listProcess(int argc, ProcessTable prtable);
int searchProcess(ProcessTable prtable, DWORD processId);
void killProcess(int argc, char *argv[], ProcessTable *prtable_ptr);
void stopProcess(int argc, char *argv[], ProcessTable prtable);
void resumeProcess(int argc, char *argv[], ProcessTable prtable);

#endif // PROCESS_H
