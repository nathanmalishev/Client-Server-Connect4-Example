//Nathan Malishev - nmalishev - 637 410 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>

#include "connect4.h"

int main(int argc, char**argv)
{
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//connect 4 stuff
	c4_t board;
	int clientMove, serverMove;
	uint16_t clientMoveNetwork, serverMoveNetwork;
	

	if (argc < 3) 
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);

	
	/* Translate host name into peer's IP address ;
	 * This is name translation service by the operating system 
	 */
	server = gethostbyname(argv[1]);
	
	if (server == NULL) 
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	
	/* Building data structures for socket */

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);

	serv_addr.sin_port = htons(portno);

	/* Create TCP socket -- active open 
	* Preliminary steps: Setup: creation of active open socket
	*/
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) 
	{
		perror("ERROR opening socket");
		exit(0);
	}
	
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
	{
		perror("ERROR connecting");
		exit(0);
	}

	srand(RSEED);
	init_empty(board);
	print_config(board);
	while((clientMove = get_move(board)) != EOF)
	{
		clientMoveNetwork = htons(clientMove);
		if(send(sockfd, &clientMoveNetwork, sizeof(uint16_t) , 0) != sizeof(uint16_t))
		{
            printf("socket closed\n");
            break;
        }

		if (do_move(board, clientMove, YELLOW)!=1) {
			printf("Panic\n");
			exit(EXIT_FAILURE);
		}
		// printf("User clientMove is %d", clientMove);
		// print_config(board);
		//now did that win?	
		if (winner_found(board) == YELLOW) {
			/* rats, the person beat us! */
			print_config(board);
			printf("Ok, you beat me, beginner's luck!\n");
			exit(EXIT_SUCCESS);
		}

		//recieve server move
		if(recv(sockfd, &serverMove, sizeof(uint16_t), 0 ) != sizeof(uint16_t))
		{
            printf("socket closed\n");
            break;
        }
        serverMoveNetwork = htons(serverMove);

		// printf("server move is: %d\n", serverMove);
		if (do_move(board, serverMoveNetwork, RED)!=1) {
			printf("Panic\n");
			exit(EXIT_FAILURE);
		}
		print_config(board);
		if (winner_found(board) == RED) {
			/* yes!!! */
			printf("I guess I have your measure!\n");
			exit(EXIT_SUCCESS);
		}
		/* otherwise, the game goes on */
		
	}
	return 0;
}
