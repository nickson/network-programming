
// For a server with more than one network cards / IP addresses - enter the IP address to bind the server socket to.
// Example:
// ./TCP_DNSserver.out 192.168.1.2
// If no IP address is entered as a parameter, binds to all IP addresses of the server.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

struct in6_addr my_addr;
struct sockaddr_in6 my_sock;

int my_fd, new_fd, numbytes;
char buf[255];
char ans[33];
struct hostent *query;
#define PORT "3333";

int main(int argc, char *argv[])
{

	int my_fd=socket(PF_INET6,SOCK_STREAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}
	
	memset(&my_sock, 0, sizeof(my_sock));
	if(argc==1){
		my_sock.sin6_family = PF_INET6;
		my_sock.sin6_addr = in6addr_any;
		my_sock.sin6_port = htons(3333); //PORT;
	}
	else{
		if (inet_pton(AF_INET6, argv[1], &(my_sock.sin6_addr)) != 1) {

			fprintf(stderr, "Invalid address\n");
			return 1;
		}
		my_sock.sin6_family = PF_INET6;
		my_sock.sin6_port = htons(3333); //PORT;
		
		char ipinput[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &(my_sock.sin6_addr), ipinput, INET6_ADDRSTRLEN);
		printf("IP Address = %s\n", ipinput);

	}
	if( bind(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1){
		perror("bind");
		return 1;
	}
	//sleep(15);
	if (listen(my_fd, 10) == -1) {
		perror("listen");
		return 1;
	}
	struct sockaddr_storage their_addr;
	socklen_t sin_size = sizeof(their_addr);
	
	while(1){
		new_fd = accept(my_fd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			return 1;
		}
		if (!fork()) { // this is the child process
			close(my_fd); // child doesn't need the listener
			if ((numbytes = recv(new_fd, buf, 254, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			query=gethostbyname(buf);
			inet_ntop(query->h_addrtype, query->h_addr_list[0], ans,  sizeof ans); 

			if (send(new_fd, ans, 20, 0) == -1)
			perror("send");
			close(new_fd);
			exit(0);
		}
	}
	//	close(my_fd);
	//	return 0;
}
