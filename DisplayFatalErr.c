// Simple Windows Sockets Echo Client and Server
// Sample solution for CS 2690 Program 1 Assignment
// Dave Heldenbrand 6/29/12
// 
// This is the error reporting function all programming assignments
//
// This program is coded in conventional C programming style, with the exception
// of the C++-style comments

// Minimum required header files for C Winsock program
#include <stdio.h>      // for print functions
#include <stdlib.h>     // for exit() 
#include <winsock2.h>    // for WSAGetLastError() 

void DisplayFatalErr(char *errMsg)
{
    fprintf(stderr,"%s: %d\n", errMsg, WSAGetLastError());  // Returns error code from current task or thread
	WSACleanup();
    exit(1);
}