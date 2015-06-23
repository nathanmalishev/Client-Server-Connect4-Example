## Project 2 make file attempt
## Nathan Malishev 637410

## CC  = Compiler.
## CFLAGS = Compiler flags.
CC     = gcc
CFLAGS = -Wall -g 


## SRV_OBJ = Server Object files.
## SRV = Server source files.
## EXE = Executable name.

SRV     =		server.c connect4.c log.c
SRV_OBJ =		server.o connect4.o log.o
SRV_EXE = 		server

CLI     =       client.c connect4.c
CLI_OBJ =       client.o connect4.o
CLI_EXE =       client

##try to implement all from a stack overflow qns 
## http://stackoverflow.com/questions/5950395/makefile-to-compile-multiple-c-programs
all: $(SRV_EXE) client


## Top level target is executable.
server:	$(SRV_OBJ)
		$(CC) $(CFLAGS) -o $(SRV_EXE) $(SRV_OBJ) -lpthread

##client compile
client: $(CLI_OBJ)
		$(CC) $(CFLAGS) -o $(CLI_EXE) $(CLI_OBJ)

## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(SRV_OBJ) 
		/bin/rm $(CLI_OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(SRV_EXE) 
		/bin/rm $(CLI_EXE) 

## Dependencies

server.o:	server.h log.c
