
// За сървър с няколко мрежови карти / IP адреса - да се укаже на кой IP адрес ще слуша.
// Пример:
// ./TCP_DNSserver.out 192.168.1.2
// Ако не се укаже на кой IP адрес ще слуша като параметър, слуша на всички

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

#define PORT 3333

struct in_addr my_addr;
struct sockaddr_in my_sock;
struct sockaddr_storage their_addr;
socklen_t sin_size = sizeof(their_addr);

int my_fd, new_fd, numbytes;
char buf[255];
char ans[33];
struct hostent *query;

int main(int argc, char *argv[])
{

	int my_fd=socket(PF_INET,SOCK_STREAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}
	
	memset(&my_sock, 0, sizeof(my_sock));
	if(argc==1){
		my_sock.sin_family = PF_INET;
		my_sock.sin_addr.s_addr = INADDR_ANY;
		my_sock.sin_port = htons(PORT); 
	}
	else{
		if (inet_pton(PF_INET, argv[1], &(my_sock.sin_addr)) != 1) {

			fprintf(stderr, "Invalid address\n");
			return 1;
		}
		my_sock.sin_family = PF_INET;
		my_sock.sin_port = htons(PORT);
		
		char ipinput[INET_ADDRSTRLEN];
		inet_ntop(PF_INET, &(my_sock.sin_addr), ipinput, INET_ADDRSTRLEN);
		printf("IP Address = %s\n", ipinput);

	}
	if( bind(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1){
		perror("bind");
		return 1;
	}
	
	if (listen(my_fd, 10) == -1) {
		perror("listen");
		return 1;
	}
	
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
