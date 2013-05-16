/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int sockfd, portno;
socklen_t clilen;
int numbers[1000];
struct sockaddr_in serv_addr, cli_addr;
int n;
int i = 0;
int sum = 0;
int client = 0;
int totalSum = 0;
int data[3];
pthread_t threads[100];
pthread_mutex_t mutexdata;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void runClient(void *arg) {
	
	int newsockfd=(int)(arg);
			printf("newsocket is %d\n",newsockfd);
	n = read(newsockfd, numbers, sizeof(numbers));
	if (n < 0) error("ERROR reading from socket");

	pthread_mutex_lock (&mutexdata);
	
	int j = 0;
	sum = 0;
	while (numbers[j] != -1){
		sum += numbers[j];
		j++;
	}
	
	data[0] = sum;
	totalSum += data[0];
	data[1] = totalSum;
	client++;
	data[2] = client;
	
	pthread_mutex_unlock (&mutexdata);	

	printf("WRIte 1\n");
	//printf("newsocket is %d\n",newsockfd
	n = write(newsockfd, data, sizeof(data));
		printf("WRIte 2\n"); 
	
	if (n < 0) error("ERROR writing to socket");
	
	//close(*newsockfd);
	pthread_exit((void*) 0);
		
}		
	
int main(int argc, char *argv[])
{	
	int newsockfd;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
			error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
		 
	while(1) {
		newsockfd = accept(sockfd, 
					(struct sockaddr *) &cli_addr, 
					&clilen);
		i++;
					
		printf("i = %d \n", i);
		pthread_create(&threads[i], NULL, (void *) runClient, (void*)newsockfd);

	}
    return 0; 
}
