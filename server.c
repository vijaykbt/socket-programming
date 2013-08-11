#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>

int main(int argc, char *argv[]) {

	int	servSockId, clntSockId;
	struct sockaddr_in	*servSockAddr, *clntSockAddr;
	int servPort, clntPort;
	int retStatus;

	servSockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(servSockId != -1) {
		fprintf(stdout, "Server Socket Created : %d \n", servSockId);
	} else {
		fprintf(stderr, "Server Socket Creation Failed!\n");
		exit(1);
	}
	
	return 0;

}
