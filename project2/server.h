// Nathan Malishev - nmalishev - 637410

#ifndef SERVER_HEADER
#define SERVER_HEADER

#define MAXIMUM_CLIENTS 120

#define SERVER 0
#define CLIENT 1

#define STATUS_ABNORMAL   1
#define STATUS_USER_WON   5
#define STATUS_DRAW       6
#define STATUS_AI_WON     7

//Thread specific information
struct readThreadParams {
    int sock;
    FILE *filep;
    struct sockaddr_in clientA, servA;
};



void *thread_game(void *threadInfo);

#endif