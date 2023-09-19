#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		perror("user: <Port number>");
		exit(1);
	}
	
	char *ip = "127.0.0.1";    // ipaddress
	
	int port = atoi(argv[1]);	// converting string to int  (argv[1] is port number)
	
	int sockfd;	 // file discriptor
	
	struct sockaddr_in server_addr;
	char buffer[1024];
	socklen_t addr_size;
	
	int n;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);		// sock_dgram is for udp
	
	if(sockfd < 0)
	{
		perror("socket not created!!!");
		exit(1);
	
	}
	
	
	bzero((char*)& server_addr, sizeof(server_addr));   // clears everything in the server_addr
	
	// setting up address for the server 
	server_addr.sin_family = AF_INET;     // for ipv4
	
	// htons() is a function used to convert the port number to network byte order, 
	// which is important for network communication.
	server_addr.sin_port = htons(port);   // giving the port number
	// inet_addr()  function convert the ip into appropriate byte format
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	
	
	
	// UDP  is musch faster as compare to the TCP as we don't need to make any connection between clinet and the server
	
	
	bzero(buffer , 1024);    // so that there is no garbage value
	
	strcpy(buffer, "hello world, life sucks");
	
	// 0 is for the default behaviour of sendto() function
	sendto(sockfd, buffer , 1024, 0, (struct sockaddr *) & server_addr, sizeof(server_addr));
	
	printf("send: %s \n", buffer);
	
	
	
	
	////////////////
	while(1)
	{
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		n = sendto(sockfd, buffer , 1024, 0, (struct sockaddr *) & server_addr, sizeof(server_addr));
	
		
		if(n<0)
		{
			perror("error while writing message");
		}
		
		
		bzero(buffer, 1024);
		
		n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &server_addr, &addr_size);
		
		
		if(n<0)
		{
			perror("error while reading message");
		}
		
		printf("Server: %s", buffer);
		
		int i = strncmp("Bye", buffer, 3);    // string compare , compare two strings 
		
		if(i == 0)
		{
			break;
		}
		
	}
	


	return 0;
}


