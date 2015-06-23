# Client-Server-Connect4-Example
Basic connect 4 client/server program written for Computer Systems (COMP30023) Third year Melbourne University Subject.
Basic server handles multiple clients connecting and playing simultaneously this concurrency is achieved through threads.
The program also makes use of thread mutex's to handle persistancy across the log file.


Usage:
Must run a server & then a client 
server port_number
client server_ip server_port_number

Example
./server 3000
./client localhost 3000

