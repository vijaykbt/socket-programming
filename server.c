#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<strings.h>

int main(int argc, char *argv[]) {

	int	servSockId, clntSockId;
	struct sockaddr_in	servSock;
	int servPort;
	int retStatus;

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

	/* Initialize Server sockaddr_in */
	servPort = atoi(argv[1]);
	bzero(&servSock, sizeof(servSock));
	servSock.sin_family = AF_INET;
	servSock.sin_addr.s_addr = htonl(INADDR_ANY);
	servSock.sin_port = htons(servPort);

	/* Bind server sock */
	retStatus = bind(servSockId, (struct sockaddr *) &servSock, sizeof(servSock));
	if(retStatus != -1) {
		fprintf(stdout, "Server Socket Bound to addr : %u, port : %d\n", servSock.sin_addr.s_addr, servSock.sin_port);
	} else {
		fprintf(stderr, "Couldn't bind server socket\n");
		exit(1);
	}
	return 0;

}
