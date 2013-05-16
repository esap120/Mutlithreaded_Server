#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

	int data[3];
	int number;
	int numbers[1000];
	bzero(numbers, sizeof(numbers));
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("sockfd is %d\n",sockfd);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	int i;
	for (i = 0; i < 1000; i++) {
		printf("> ");
		scanf("%d", &number);
		if (number > 0) { 
			numbers[i] = number;
		}
		else break;
	}
	numbers[i] = -1;
		
	n = write(sockfd, numbers, sizeof(numbers));
    if (n < 0) 
         error("ERROR writing to socket");
	printf("reading \n");
	n = read(sockfd, data, sizeof(data));
	
	printf("reading done \n");
    if (n < 0) 
         error("ERROR reading from socket");

	printf("The sum is: %d \n", data[0]);
	printf("The current Grand Total is %d and I have served %d clients so far! \n", data[1], data[2]);
	
	close(sockfd);
	
	return 0;
}
