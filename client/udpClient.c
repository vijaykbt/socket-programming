#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<strings.h>
#include<string.h>

#define MAXBUF 1024

int main(int argc, char *argv[]) {

	int	clntSockId;
	struct sockaddr_in	servSockAddr, clntSockAddr;
	int servPort;
	int retStatus;
	int servSockLen;
	char buf[MAXBUF];

	/* Check for cmd line options */
	if (argc != 3) {
		fprintf(stderr, "Usage : %s <ServerAddr> <ServerPort>\n", argv[0]);
		exit(1);
	}
	
	/* Create Client Socket */
	clntSockId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(clntSockId != -1) {
		fprintf(stdout, "Client Socket Created : %d \n", clntSockId);
	} else {
		fprintf(stderr, "Client Socket Creation Failed!\n");
		exit(1);
	}

	/* Initialize Server sockaddr_in. User INADDR_ANY to bind to
		 all local addresses */
	servPort = atoi(argv[2]);
	bzero(&servSockAddr, sizeof(servSockAddr));
	servSockAddr.sin_family = AF_INET;
	inet_addr(argv[1], servSockAddr.sin_addr.s_addr);
	servSockAddr.sin_port = htons(servPort);
	servSockLen = sizeof(servSockAddr);

	strcpy(buf, "Hello from client\n");
	/* Send data to server */
	retStatus = sendto(clntSockId, buf, strlen(buf)+1, 0, 
									(struct sockaddr *) &servSockAddr, 
									 sizeof(servSockAddr));
	if(retStatus > 0) {
		fprintf(stdout, "Message sent to server\n"); 
	} else {
		fprintf(stderr, "Couldn't send message to server\n");
		close(clntSockId);
		exit(1);
	}

	/* Receive data from server */
	bzero(&buf, MAXBUF);
	retStatus = recvfrom(clntSockId, buf, MAXBUF, 0, 
									(struct sockaddr *)&servSockAddr, &servSockLen);
	if(retStatus > 0) {
		fprintf(stdout, "%d Bytes received from server %s\n", retStatus, buf);
	} else {
		fprintf(stdout, "Couldn't read data from server\n");
		close(clntSockId);
		exit(0);
	}
	close(clntSockId);
	return 0;
}
