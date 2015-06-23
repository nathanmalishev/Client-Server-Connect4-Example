// Nathan Malishev - nmalishev - 637410
//Project 2 - Server/Client that Handles < 100 clients
//Sample code built upon from class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#include "connect4.h"
#include <pthread.h>
#include "server.h"
#include "log.h"

pthread_mutex_t lock;

int main(int argc, char **argv)
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    //information that is passed into the thread
    struct readThreadParams *threadInfo;
    

    if (argc < 2) 
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

     /* Create TCP socket */
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }

    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);
    
    /* Create address we're going to listen on (given port number)
     - converted to network byte order & any IP address for 
     this machine */
    
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(portno);  // store in machine-neutral format

     /* Bind address to the socket */
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("ERROR on binding");
        exit(1);
    }
    
    /* Listen on socket - means we're ready to accept connections - 
     incoming connection requests will be queued */
    
    listen(sockfd,MAXIMUM_CLIENTS);
    
    clilen = sizeof(cli_addr);
    

    FILE *fptr;
    fptr = fopen("log.txt", "a");
    if(fptr == NULL) //if file does not exist, create it
    {
        printf("error with file!\n");
    }
    


    /* Accept a connection - block until a connection is ready to
     be accepted. Get back a new file descriptor to communicate on. */


    while((newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen)) != -1)
    {
        threadInfo             = (struct readThreadParams*)malloc(sizeof(struct readThreadParams));
        threadInfo->clientA    = cli_addr;
        threadInfo->servA      = serv_addr;
        threadInfo->sock       = newsockfd;
        threadInfo->filep      = fptr;
        
        if (newsockfd < 0) 
        {
            perror("ERROR on accept");
            exit(1);
        }
        
        bzero(buffer,256);

        //we want to create a thread here
        pthread_t tid;
        //create a thread
        pthread_create(&tid, NULL, thread_game, (void*)threadInfo);
        //wait for thread
        pthread_detach(tid);
    }
    //wait for last one
    // pthread_join(tid,NULL);

    
    return 0; 
}


void *thread_game(void *threadInfo)
{   
    struct readThreadParams *thisTheadInfo = (struct readThreadParams *)threadInfo;
    FILE *fptr = thisTheadInfo->filep;
    int sock = thisTheadInfo->sock;
    int status,n;
    //connect 4 variables
    c4_t board;
    int clientMove, serverMove;
    //uint16 used so all systems can play!
    uint16_t clientMoveNewtorked, serverMoveNetworked;



    printLog(fptr,thisTheadInfo,"client connected" ,-1,CLIENT);
 
    //connect 4 actions here
    srand(RSEED);
    //create a new board
    init_empty(board);
    /* main loop does two moves each iteration, one from the human
     * playing, and then one from the computer program (or game server)
     */
    while (1) {

        //first we want the clients move
        if( (n = recv(sock, &clientMove, sizeof(uint16_t), 0)) != sizeof(uint16_t) )
        {

            printf("socket closed\n");
            status = STATUS_ABNORMAL;
            printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
            break;
        }
        clientMoveNewtorked = htons(clientMove);
        //do move
        if (do_move(board, (int)clientMoveNewtorked, YELLOW)!=1) {
            printf("Panic\n");
            status = STATUS_ABNORMAL;
            printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
            break;
        } 
        //if move possible we write to log
        printLog(fptr, thisTheadInfo, "client's move", (int)clientMoveNewtorked,CLIENT);

        // print_config(board);
        if (winner_found(board) == YELLOW) {
            /* rats, the person beat us! */
            printf("Petty human wins\n");
            status = STATUS_USER_WON;
            printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
            break;
        }

        /* was that the last possible move? */
         if (!move_possible(board)) {
             /* yes, looks like it was */
             printf("An honourable draw\n");
             status = STATUS_DRAW;
             printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
             break;
         }
         /* otherwise, look for a move from the computer */
         serverMove = suggest_move(board, RED);

         serverMoveNetworked = htons(serverMove);
         /* then play the move */
         // printf(" I play in column %d\n", serverMove);
         if(send(sock, &serverMoveNetworked,sizeof(uint16_t),0) != sizeof(uint16_t))
         {
            printf("error\n");
            status = STATUS_ABNORMAL;
            printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
            break;
         }
         if (do_move(board, serverMove, RED)!=1) {
             printf("Panic\n");
             status = STATUS_ABNORMAL;
             printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
             break;
         }
         
         printLog(fptr, thisTheadInfo, "server's move", serverMove,SERVER);

         // print_config(board);
         /* and did we win??? */
         if (winner_found(board) == RED) {
             /* yes!!! */
             printf("Server wins again!\n");
             status = STATUS_AI_WON;
             printLog(fptr, thisTheadInfo, "game over, code",status,CLIENT);
             break;
             
         }
         /* otherwise, the game goes on */
        


    }
     

    return NULL;
}