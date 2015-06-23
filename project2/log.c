
//Nathan Malishev - nmalishev - 637 410 

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "log.h"
#include "server.h"
#include <pthread.h>

//get lock from an external file
extern pthread_mutex_t lock;

//not the best use of abstraction but for this assignment & time constraints it
//will have to do
void printLog(FILE *fp, struct readThreadParams *threadInfo,char *message, int code , int flag){

	//lock this method - we don't want anything else messing with log file
	pthread_mutex_lock(&lock);

	struct sockaddr_in ca = threadInfo->clientA;
	struct sockaddr_in sa = threadInfo->servA;
	char strInt[20];
	//set up buffers and allocate memory
	char *buffer;
	buffer = (char *)malloc(sizeof(char)*250);
	char *buffer2;
	buffer2 = (char *)malloc(sizeof(char)*250);
	//get the time and append to buffer
	strcpy(buffer, currentTime());
	strcat(buffer, "]");
	

	//Get Ip address and add to buffer
	if(flag == CLIENT){
		
		inet_ntop(AF_INET, &(ca.sin_addr), buffer2, INET_ADDRSTRLEN);
		strcat(buffer, "(");
		strcat(buffer, buffer2);
		strcat(buffer, ")");

		//Get Sock Num turn it into number and add it to buffer
		strcat(buffer, "(soc_id ");
		sprintf(strInt, "%d", threadInfo->sock);
		strcat(buffer,  strInt);
		strcat(buffer, ") ");
	}else{
		//if not client must be server
		inet_ntop(AF_INET, &(sa.sin_addr), buffer2, INET_ADDRSTRLEN);
		strcat(buffer, "(");
		strcat(buffer, buffer2);
		strcat(buffer, ") ");
	}

	//add rest to buffer and print
	//possibly vunerable if they fill upmessage past buffer overflow?
	strcat(buffer, message);
	//does the code need to be added?
	if(code != -1){
		strcat(buffer, " = ");
		sprintf(strInt, "%d", code);
		strcat(buffer, strInt);
	}

	strcat(buffer, "\n");
	fprintf(fp, buffer);

	fflush(fp);
	//unlock the thread and let anyone use log.txt
	pthread_mutex_unlock(&lock);
}

//retrieve the current time
char * currentTime(){
	char *buff;
	buff = (char*)malloc(sizeof(char)*19);
	time_t now = time(NULL);
	strftime(buff, 22, "[%Y-%m-%d, %H:%M:%S", localtime(&now));

	return buff;
}
