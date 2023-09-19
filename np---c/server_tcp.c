#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error (const char* msg)   // to display a message and terminate the program
{
	perror(msg);
	exit(1);
}



// argc is the number of command line argument passed 
// argv will contain the passed parameters 
// in our case we will be passing 2 parameters , file name
// and port number 

int main(int argc, char* argv[])    
				    
{
	if(argc<2)  		// if the port number is not provided 
	{
	    fprintf(stderr, "port number not provinded, program terminated!");
	    exit(1);
	} 
	
	int sockfd;    // file discriptor
	int newsockfd ; // after the link has been established
	int portno;   // number
	int n ;
	char buffer[255] ;    // to stor messages that we want to pass to the client
	
	// this stores the internet address for the server and cilent
	struct sockaddr_in serv_add , cli_add;	// included in file netinet.h
	
	socklen_t cli_len;     
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	// AF_INET is for ipv4
	// SOCK_STREAM  is for tcp
	// 0 is to tell to use defalult protocol 
	
	if(sockfd < 0)
	{
		error("error opening socket");
	}
	
	
	bzero((char*) & serv_add, sizeof(serv_add)); //this clears everything so that we don't face any problem
	
	portno = atoi(argv[1]);	// converts the string into integer 
	
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY; 
	serv_add.sin_port = htons(portno);
	
	// sockaddr is diffrent form sockadd_in
	if(bind(sockfd, (struct sockaddr*) & serv_add, sizeof(serv_add)) < 0)
	{
		error("binding failed");
	}
	
	
	// server is listening 
	listen(sockfd, 5);	//5 is the max no. of clients you can connect
	
	cli_len = sizeof(cli_add);
	
	// now we accept the connection
	
	newsockfd = accept(sockfd, (struct sockaddr*) &cli_add, &cli_len);

	if(newsockfd < 0)
	{ 	
		error("error on accepting the client");
	}
	
	
	// to make a chat box
	
	while(1)
	{
		bzero(buffer, 255);    // 255 is size of the buffer
		// this read function is for server then there is write function in client side too	
		n = read(newsockfd, buffer, 255);
		
		if(n< 0)
		{
			error("error while reading");
		}
		
		printf("Client: %s\n", buffer);
		
		bzero(buffer, 255);    
		
		// from stdio.h , reads bytes from the stream
		fgets(buffer, 255, stdin);
		
		// now fgets will take reply form the server and send it to the client
		
		n = write(newsockfd, buffer, strlen(buffer));
		
		if (n< 0)
		{
			error("error while writing");
		}
		
		int i = strncmp("Bye", buffer, 3);    // string compare , compare two strings 
		
		if(i == 0)
		{
			break;
		}
	}
	
	
	close(newsockfd);
	close(sockfd);
	return 0;
	
	
	
}
