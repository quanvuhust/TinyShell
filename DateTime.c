#include "DateTime.h"
#include "TinyShell.h"
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
void changeDate (SYSTEMTIME date)
{
    char Str[DATELEN + 2];
    register int Count, flag, flag_digit;
    WORD temp_word, i;

    while(1){
        printf("Enter the new date: (dd-mm-yy) ");
        CleanBuffer();
        fgets(Str, DATELEN + 3, stdin);

        int len = strlen(Str);
        Str[len - 1] = '/';
        Str[len] = '\0';
        if(Str[0] == '/' && len == 1){
            printf("\n");
            break;
        }

        temp_word = 0; i = 0, flag = 1, Count = 0, flag_digit = 0;
        while(Str[i] != '\0'){
            if(isdigit(Str[i])){
                flag_digit = 1;
                temp_word = Str[i] - ZERO + temp_word*10;
            }
            else if(Str[i]=='/'){
                Count++;
                if      ((Count == 1) && (temp_word < 32))
                    date.wDay = temp_word;
                else if ((Count == 2) && (temp_word < 13)){
                    date.wMonth = temp_word;
                    switch (date.wDay){
                        case JAN:
                        case MAR:
                        case MAY:
                        case JUL:
                        case AUG:
                        case OCT:
                        case DEC: break;
                        default:{
                            if((date.wDay == 31) || (temp_word == FEB && date.wDay > 29)){
                                Str[i + 1] = '\0';//To end while loop
                                _TERMINATE_
                            }
                        }
                    }
                }
                else if ((Count == 3) && (temp_word < MAXYEAR)){
                    if(temp_word % 4 != 0 && date.wMonth == FEB && date.wDay == 29){
                        _TERMINATE_
                    }
                    else
                        date.wYear = temp_word;
                }
                else{
                    _TERMINATE_
                }
                temp_word = 0;
            }
            else{
                _TERMINATE_
            }
            i++;
        }

        if(flag && flag_digit && Count == 3){
            if(!SetLocalTime(&date))
            	puts("A required privilege is not held by the client.");
            break;
        }
        else
            printf("The system cannot accept the date entered.\n");
    }
}

void changeTime (SYSTEMTIME time)
{
    char Str[TIMELEN + 2];
    register int Count;
    WORD temp_word, i, flag, flag_digit;

    while(1){
        printf("Enter the new time: ");
        CleanBuffer();
        fgets(Str, TIMELEN + 3, stdin);

        int len = strlen(Str);
        Str[len - 1] = ':';
        Str[len] = '\0';
        if(Str[0] == ':' && len == 1){
            printf("\n");
            break;
        }

        temp_word = 0; i = 0, flag = 1, Count = 0, flag_digit = 0;
        while(Str[i] != '\0'){
            if(isdigit(Str[i])){
                flag_digit = 1;
                temp_word = Str[i] - ZERO + temp_word*10;
            }
            else if(Str[i]==':'){
                Count++;
                if      ((Count == 1) && (temp_word < 24)){
                    time.wHour = temp_word;
                    time.wMinute = 0;
                    time.wSecond = 0;
                }
                else if ((Count == 2) && (temp_word < 60))
                    time.wMinute = temp_word;
                else if ((Count == 3) && (temp_word < 60))
                    time.wSecond = temp_word;
                else{
                    _TERMINATE_
                }
                temp_word = 0;
            }
            else{
                _TERMINATE_
            }
            i++;
        }

        if(0 < Count && Count < 4 && flag && flag_digit){
            if(!SetLocalTime(&time))
            	puts("A required privilege is not held by the client.");
            break;
        }
        else
            printf("The system cannot accept the time entered.\n");
	}
}

void dateCommand(int argc)
{
    if (argc > 1){
        printf("Too many arguments to command date.\n");
        return;
    }

    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("The current date is: %02u/%02u/%04u\n", st.wDay, st.wMonth, st.wYear);
    changeDate(st);
}

void timeCommand(int argc)
{
    if (argc > 1){
        printf("Too many arguments to command time.\n");
        return;
    }

    SYSTEMTIME st;
    GetLocalTime(&st);;
    printf("The current time is: %02u:%02u:%02u,%u\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds / 10);
    changeTime(st);
}
