#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error (const char* msg)   // to display a message and terminate the program
{
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[])    
				    
{
	if(argc<2)  		// if the port number is not provided 
	{
	    fprintf(stderr, "Not enough aruments, program terminated!");
	    exit(1);
	} 
	
	int sockfd;   
	int portno = atoi(argv[1]);   
	int n ;
	char buffer[255] ;    
	
	// this stores the internet address for the server and cilent
	struct sockaddr_in serv_add;	
	socklen_t cli_len;     
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	
	if(sockfd < 0)
	{
		error("error opening socket");
	}
	

	bzero((char *) & serv_add, sizeof(serv_add));
	
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_add.sin_port = htons(portno);
	
	
	// connecting to the server
	
	if(connect(sockfd, (struct sockaddr *) &serv_add, sizeof(serv_add)) <0)
	{
		error("connection fffffailed");
	}
	
	
	// while loop for communication
	
	while(1)
	{
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, sizeof(buffer));
		
		if(n<0)
		{
			error("error while writing message");
		}
		
		
		bzero(buffer, 255);
		
		n = read(sockfd, buffer, 255);
		
		if(n<0)
		{
			error("error while reading message");
		}
		
		printf("Server: %s", buffer);
		
		int i = strncmp("Bye", buffer, 3);    // string compare , compare two strings 
		
		if(i == 0)
		{
			break;
		}
		
		close(sockfd);
		return 0;
	
	}
}


