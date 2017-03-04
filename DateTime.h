#ifndef DATETIME_H
#define DATETIME_H

#include <windows.h>
#define TIMELEN 8
#define ZERO '0'
#define DATELEN 10
#define MAXYEAR 10000
#define _TERMINATE_ \
    flag = 0;       \
    break;
#define NUMBER_DATETIME_COMMANDS 2

enum MONTH {JAN = 1, FEB = 2, MAR = 3, APR = 4 , MAY = 5 , JUN = 6,
           JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12};

void changeDate (SYSTEMTIME date);
void changeTime (SYSTEMTIME time);
void dateCommand(int argc);
void timeCommand(int argc);

#endif // DATETIME_H
