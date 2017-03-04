#include "Process.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <windows.h>

BOOL CtrlHandler(DWORD fdwCtrlType)
{
    switch( fdwCtrlType ){
        case CTRL_C_EVENT:
            return( TRUE );
        default:
            return FALSE;
    }
}

int modeBackground(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram,
                   char* commandlineArguments)
{
    return CreateProcess(nameProgram, commandlineArguments, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, si_ptr, pi_ptr);
}

int modeForeground(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram,
                   char* commandlineArguments)
{
    int flag = 0;
    flag = CreateProcess(nameProgram, commandlineArguments, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, si_ptr, pi_ptr);
    if(flag) {
        HANDLE hprocess = pi_ptr->hProcess;
        SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE );
        WaitForSingleObject(hprocess, INFINITE);
        TerminateProcess(hprocess, 0);
        CloseHandle(hprocess);
        CloseHandle(pi_ptr->hThread);
        return 1;
    } else
        return 0;
}

PROCESS_INFORMATION createProcess(const char* nameProgram, char* commandlineArguments,
                                  int (*modeProcess)(STARTUPINFO *si_ptr, PROCESS_INFORMATION *pi_ptr, const char* nameProgram, char* commandlineArguments))
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    if(!modeProcess(&si, &pi, nameProgram, commandlineArguments)) {
        printf("Create process failed");
        exit(1);
    }

    return pi;
}

int checkStatusThread(HANDLE hThread)
{
    if(!SuspendThread(hThread)){
        ResumeThread(hThread);
        return 1;
    }
    else{
        ResumeThread(hThread);
        ResumeThread(hThread);
        SuspendThread(hThread);
        return 0;
    }

}

void listProcess(int argc, ProcessTable prtable)
{
    if (argc > 1) {
        printf("Too many arguments to command list.\n");
        return;
    }

    int i;
    TCHAR Buffer[MAX_PATH];
    char status[9];
    if(prtable.NumberProcess == 0) {
        printf("Don't have any process.\n");
        return;
    }
    printf("\n\t\tLIST OF PROCESS\n");
    for(i = 0; i < prtable.NumberProcess; i++) {
        GetModuleFileNameEx(prtable.Table[i].hProcess, 0, Buffer, MAX_PATH);
        if(checkStatusThread(prtable.Table[i].hThread))
            strcpy(status, "Running");
        else
            strcpy(status, "Stopping");

        printf("No. %d:\n", i + 1);
        printf("\tProcess Name: %s\n", Buffer);
        printf("\tProcessID: %lu\n", prtable.Table[i].dwProcessId);
        printf("\tStatus: %s\n", status);
    }
}

int searchProcess(ProcessTable prtable, DWORD processId)
{
    int i;
    for(i = 0; i < prtable.NumberProcess; i++)
        if(processId == prtable.Table[i].dwProcessId)
            return i;
    return -1;
}

void stopProcess(int argc, char *argv[], ProcessTable prtable)
{
    if(argc == 1) {
        printf("Too few arguments to command stop.\n");
        return;
    } else if (argc > 2) {
        printf("Too many arguments to command stop.\n");
        return;
    }

    DWORD processId = atoi(argv[1]);
    int index = searchProcess(prtable, processId);
    if(index != -1)
        SuspendThread(prtable.Table[index].hThread);
    else
        printf("Invalid process ID.\n");
}

void resumeProcess(int argc, char *argv[], ProcessTable prtable)
{
    if(argc == 1) {
        printf("Too few arguments to command resume.\n");
        return;
    } else if (argc > 2) {
        printf("Too many arguments to command resume.\n");
        return;
    }

    DWORD processId = atoi(argv[1]);
    int index = searchProcess(prtable, processId);
    if(index != -1)
        ResumeThread(prtable.Table[index].hThread);
    else
        printf("Invalid process ID.\n");
}

void deleteElement(PROCESS_INFORMATION table[], int position, int len)
{
    int i;
    for(i = position; i < len - 1; i++)
        table[i] = table[i + 1];
}

void killProcess(int argc, char *argv[], ProcessTable *prtable_ptr)
{
    if(argc == 1) {
        printf("Too few arguments to command kill.\n");
        return;
    } else if (argc > 2) {
        printf("Too many arguments to command kill.\n");
        return;
    }

    DWORD processId = atoi(argv[1]);
    int index = searchProcess(*prtable_ptr, processId);
    if(index != -1) {
        HANDLE hprocess = prtable_ptr->Table[index].hProcess;
        TerminateProcess(hprocess, 0);
        deleteElement(prtable_ptr->Table, index, prtable_ptr->NumberProcess--);
        CloseHandle(hprocess);
        CloseHandle(prtable_ptr->Table[index].hThread);
    } else
        printf("Invalid process ID.\n");
}

