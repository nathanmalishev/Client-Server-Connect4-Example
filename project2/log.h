//Nathan Malishev - nmalishev - 637 410 
//header for log file

#ifndef LOG_HEADER
#define LOG_HEADER

#include <stdio.h>
#include "server.h"

char * currentTime();
void printLog(FILE *fp, struct readThreadParams *threadInfo,char *message, int code , int flag);


#endif
