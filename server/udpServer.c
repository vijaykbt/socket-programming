#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<strings.h>
#include<string.h>

#define MAXBUF 1024

int main(int argc, char *argv[]) {

	int	servSockId, clntSockId;
	struct sockaddr_in	servSockAddr, clntSockAddr;
	int servPort, clntAddrLen;
	int retStatus;
	char buf[MAXBUF];

	/* Check for cmd line options */
	if (argc != 2) {
		fprintf(stderr, "Usage : %s <ServerPort>\n", argv[0]);
		exit(1);
	}
	
	/* Create Server Socket */
	servSockId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

	/* Receive data from clients */
	while(1) {
		bzero(&clntSockAddr, sizeof(clntSockAddr));
		clntAddrLen = sizeof(clntSockAddr);
		retStatus = recvfrom(servSockId, buf, MAXBUF, 0, 
								(struct sockaddr *) &clntSockAddr, &clntAddrLen);
		if(retStatus == -1) {
			fprintf(stderr, "Couldn't receive message\n");
		} else {
			fprintf(stdout, "Received message from client - %u:%d\n", 
						clntSockAddr.sin_addr.s_addr, clntSockAddr.sin_port);
			fprintf(stdout, "Message : %s\n", buf);
			bzero(&buf, MAXBUF);
			strcpy(buf, "Hello from server");
			/* Send message to client */
			retStatus = sendto(servSockId, buf, strlen(buf)+1, 0,
									(struct sockaddr *) &clntSockAddr, clntAddrLen);
			if(retStatus > 0) {
				fprintf(stdout, "Wrote %d bytes to client\n", retStatus);
			} else {
				fprintf(stderr, "Error in writing data to client\n");
			}
		}
	}
	close(servSockId);
	return 0;

}
