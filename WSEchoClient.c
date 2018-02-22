// CS 2690 Program 1 
// Simple Windows Sockets Echo Client
// Last update: 02/13/18
// Isaiah Larsen 601 02/19/18
// <Windows 10 visual studio community 2017>
//
// Usage: WSEchoClient <server IP address> <server port> <"message to echo">
// Companion server is WSEchoServer
// Server usage: WSEchoServer <server port>
//
// This program is coded in conventional C programming style, with the 
// exception of the C++-style comments.
//
// I declare that the following source code was written by me, or provided
// by the instructor for this project. I understand that copying
// source code from any other source constitutes cheating, and that I will
// receive a zero grade on this project if I am found in violation of
// this policy.
// ----------------------------------------------------------------------------

// Minimum required header files for C Winsock program
#include <stdio.h>      // for print functions
#include <stdlib.h>     // for exit() 
#include <winsock2.h>   // for Windows sockets functions
#include <Ws2ipdef.h>   // the following two header files are required for our support functions
#include <Ws2tcpip.h>


// #define ALL required constants HERE, not inline (#define is a macro, don't terminate with ';')
// For example...  
#define RCVBUFSIZ  50

// declare any functions located in other .c files here
void DisplayFatalErr(char *errMsg); // writes error message before abnormal termination

void main(int argc, char *argv[]) // argc is # of strings following command, argv[] is array of ptrs to the strings
{
    // Declare ALL variables and structures for main() HERE, not inline (including the following...)
    WSADATA wsaData;                // contains details about WinSock DLL implementation
    struct sockaddr_in6 serverInfo;	// standard IPv6 structure that holds server socket info
	char *serverIPaddr;			    // ptr to char string holding IPv4 address
	unsigned short serverPort;
	int sock;						// tcp socket variable
	int msgLen;
	int i = 0;
	char *msg;
	int numArgs;
	char rcvBuf[RCVBUFSIZ];
	int perrno = -1; 
	int bytesRead;
	int v6Only = 0;

    // Verify correct number of command line arguments, else do the following:
	
	numArgs = argc;
	if (numArgs < 4 || numArgs > 4) {
		// fprintf(stderr, "Helpful error message goes here"\n");
		fprintf(stderr, "Incorrect Args");
		// exit(1);	// ...and terminte with abnormal termination code (1)
		exit(1);
	}
	
    

    // Retrieve the command line arguments. (Sanity checks not required, but port and IP addr will need
    // to be converted from char to int.  See slides 11-13 & 12-3 for details.)
	serverIPaddr = argv[1];
	serverPort = atoi(argv[2]);
	msg = argv[3];
	msgLen = strlen(msg);


    // Load the Winsock 2.0 DLL. Returns 0 if ok. If this fails, fprint error message to stderr as above and exit.  
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed");

	}


    // Create a TCP stream socket.  Now that Winsock DLL is loaded, we can signal any errors as shown on next line:
	if (socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP) == INVALID_SOCKET) {
		DisplayFatalErr("socket() function failed.");
	}else{
		sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		printf("Socket created successfully.  Press any key to continue...");
		getchar();  // needed to hold console screen open
	}

    // Zero out the sockaddr_in6 structure and load server info into it.  See slide 11-15.
	memset(&serverInfo, 0, sizeof(serverInfo));
	serverInfo.sin6_family = AF_INET6;         // address family = IPv6
	serverInfo.sin6_port = htons(serverPort);  // convert int port to network order
	// Convert server addr from char to ntwk form
	if (inet_pton(AF_INET6, serverIPaddr, &serverInfo.sin6_addr) < 0) { DisplayFatalErr("inet failure"); }
	




    // Before you connect, but after you create your socket handle you'll need to use setsockopt()
    //    to enable ipv6 and ipv4 both.  See the lecture slides for details on this call.
	perrno = setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&v6Only, sizeof(v6Only));

    // Attempt connection to the server.  If it fails, call DisplayFatalErr() with appropriate message.
	if (connect(sock, (struct sockaddr *) &serverInfo, sizeof(serverInfo)) < 0) {
		DisplayFatalErr("connect() function failed.");
	}

    printf("Connection established with server.  Press any key to continue...");
    getchar();

    // Send message to server (without \0 null terminator). Check for null msg (length=0) & verify all bytes were sent...
    // ...else call DisplayFatalErr() with appropriate message as before
	if (send(sock, msg, msgLen, 0) != msgLen) {
		bytesRead = send(sock, msg, msgLen, 0);
	}

    printf("Message transmitted to server.  Press any key to continue...");
    getchar();

    // Retrieve the message returned by server.  Be sure you've read the whole thing. 
    // Manage receive buffer to prevent overflow with a big message.
    // Call DisplayFatalErr() if this fails.  (Lots can go wrong here, see slides.)
	if ((bytesRead = recv(sock, rcvBuf, RCVBUFSIZ - 1, 0)) <= 0) {
		DisplayFatalErr("message recieved bad");
	}
	

	// Display ALL of the received message, in printable C string format.
    printf("Received from server: ");
	for (i; i < bytesRead; i++) {
		printf("%c", rcvBuf[i]);
		
	}

	printf("\n");
    printf("Ready to close the connection.  Press any key to continue...");
    getchar();

    // This closes the TCP connection (sends a FIN)
	if (closesocket(sock) != 0) {
		DisplayFatalErr("socket not closed properly");
	}

    printf("Connection to server closed.  Press any key to terminate the client.");
    getchar();

    // Release the Winsock DLL
	WSACleanup();

    exit(0);
}
