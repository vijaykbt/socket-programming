#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<strings.h>
#include<string.h>
#include<fcntl.h>

#define SERVERPORT 4444
#define MAXBUF 1024

int main(int argc, char *argv[]) {

	int	clntSockId;
	int fd;
	struct sockaddr_in	servSockAddr;
	int servPort;
	int retStatus;
	char buf[MAXBUF];

	/* Check for cmd line options */
	if (argc != 3) {
		fprintf(stderr, "Usage : %s <ServerAddr> <ServerPort>\n", argv[0]);
		exit(1);
	}
	
	/* Create Client Socket */
	clntSockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	/* Connect to server */
	retStatus = connect(clntSockId, (struct sockaddr *) &servSockAddr, 
									 sizeof(servSockAddr));
	if(retStatus == 0) {
		fprintf(stdout, "Connected to Server\n"); 
	} else {
		fprintf(stderr, "Couldn't connect to server\n");
		close(clntSockId);
		exit(1);
	}

	/* Send file name to server */
	strcpy(buf, "./tcpServer.c");
	retStatus = write(clntSockId, buf, strlen(buf)+1);
	if(retStatus > 0) {
		fprintf(stdout, "File name sent to server : %s\n", buf);
	} else {
		fprintf(stdout, "Couldn't send file name to server\n");
		close(clntSockId);
		exit(0);
	}
	bzero(buf, MAXBUF);
	retStatus = read(clntSockId, buf, MAXBUF);
	if(retStatus > 0) {
		if(strcmp(buf, "FILE FOUND") == 0) {
			fprintf(stdout, "File found on the server\n");
			fd = open("./tcpServer.c", 777);
			bzero(buf, MAXBUF);
			while((retStatus = read(clntSockId, buf, MAXBUF)) > 0) {
				write(fd, buf);
				fprintf(stdout, "Got %d bytes\n", retStatus);
				fprintf(stdout, "%s\n", buf);
				bzero(buf, MAXBUF);
			}
			close(fd);
		} else {
			fprintf(stderr, "File not found on the server\n");
			close(clntSockId);
			exit(0);
		}
	} else {
		fprintf(stderr, "Couldn't get file status from server\n");
		close(clntSockId);
		exit(0);
	}
	close(clntSockId);
	return 0;
}
