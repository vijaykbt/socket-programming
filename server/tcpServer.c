#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<strings.h>
#include<string.h>


int main(int argc, char *argv[]) {

	int	servSockId, clntSockId;
	struct sockaddr_in	servSockAddr, clntSockAddr;
	int servPort, clntAddrLen;
	int retStatus;
	const char msg[] = "Hello from server";

	/* Check for cmd line options */
	if (argc != 2) {
		fprintf(stderr, "Usage : %s <ServerPort>\n", argv[0]);
		exit(1);
	}
	
	/* Create Server Socket */
	servSockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(servSockId != -1) {
		fprintf(stdout, "Server Socket Created : %d \n", servSockId);
	} else {
		fprintf(stderr, "Server Socket Creation Failed!\n");
		exit(1);
	}

	/* Initialize Server sockaddr_in. User INADDR_ANY to bind to
		 all local addresses */
	servPort = atoi(argv[1]);
	bzero(&servSockAddr, sizeof(servSockAddr));
	servSockAddr.sin_family = AF_INET;
	servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servSockAddr.sin_port = htons(servPort);

	/* Bind server sock */
	retStatus = bind(servSockId, (struct sockaddr *) &servSockAddr, 
									 sizeof(servSockAddr));
	if(retStatus == 0) {
		fprintf(stdout, "Server Socket Bound to addr : %u, port : %d\n", 
				    servSockAddr.sin_addr.s_addr, servSockAddr.sin_port);
	} else {
		fprintf(stderr, "Couldn't bind server socket\n");
		close(servSockId);
		exit(1);
	}

	/* Listen for connections */
	retStatus = listen(servSockId, 5);
	if(retStatus == -1) {
		fprintf(stderr, "Couldn't listen for connections\n");
		close(servSockId);
	}
	fprintf(stdout, "Started listening for connections\n");

	/* Accept incoming connections */
	while(1) {
		bzero(&clntSockAddr, sizeof(clntSockAddr));
		clntAddrLen = sizeof(clntSockAddr);
		clntSockId = accept(servSockId, (struct sockaddr *) &clntSockAddr,
												&clntAddrLen);
		if(clntSockId == -1) {
			fprintf(stderr, "Couldn't accept connection\n");
			close(servSockId);
			exit(1);
		}
		fprintf(stdout, "Accepted connection from client - %u:%d\n", 
						clntSockAddr.sin_addr.s_addr, clntSockAddr.sin_port);
		/* Handle Client connection */
		retStatus = write(clntSockId, msg, strlen(msg));
		if(retStatus > 0) {
			fprintf(stdout, "Wrote %d bytes to client\n", retStatus);
		} else {
			fprintf(stderr, "Error in writing data to client\n");
			close(clntSockId);
			close(servSockId);
			exit(1);
		}
		close(clntSockId);
	}
	close(servSockId);
	return 0;

}
