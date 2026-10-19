// For a server with more than one network cards / IP addresses - enter the IP address to bind the server socket to.
// Example:
// ./serverDGRAM_IPv6.out 2001:67c:20d0:3a:216:17ff:feb2:874e
// If no IP address is entered as a parameter, binds to all IP addresses of the server.

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define SERVERPORT 3333
#define MAXDNLEN 254

char buf[MAXDNLEN];
int my_fd;
int numbytes;
struct sockaddr_in6 my_sock;
struct sockaddr_in6 client_addr;
socklen_t addr_len = sizeof client_addr;
char s[INET6_ADDRSTRLEN];
struct addrinfo hints, *infoptr;
char ans[46];   // = ans[INET6_ADDRSTRLEN], e.g. IPv4-mapped IPv6 address like ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255

int main(int argc, char *argv[])
{
	my_fd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (my_fd == -1) {
		perror("socket");
		return 1;
	}

	memset(&my_sock, 0, sizeof(my_sock));
	if (argc == 1) {
		my_sock.sin6_family = AF_INET6;
		my_sock.sin6_addr = in6addr_any;
		my_sock.sin6_port = htons(SERVERPORT);
	} else {
		if (inet_pton(AF_INET6, argv[1], &(my_sock.sin6_addr)) != 1) {
			fprintf(stderr, "Invalid address\n");
			close(my_fd);
			return 1;
		}
		my_sock.sin6_family = AF_INET6;
		my_sock.sin6_port = htons(SERVERPORT);

		char ipinput[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &(my_sock.sin6_addr), ipinput, INET6_ADDRSTRLEN);
		printf("IP Address = %s\n", ipinput);
		fflush(stdout);
	}

	if (bind(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1) {
		perror("bind");
		close(my_fd);
		return 1;
	}

	while (1) {
		addr_len = sizeof client_addr;
		if ((numbytes = recvfrom(my_fd, buf, MAXDNLEN - 1, 0, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
			perror("recvfrom");
			return 1;
		}

		printf("listener: got packet from %s\n",
			inet_ntop(client_addr.sin6_family, &client_addr.sin6_addr, s, sizeof s));
		printf("listener: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET6; // Force IPv6 AAAA record resolution
		hints.ai_socktype = SOCK_STREAM;

		int result = getaddrinfo(buf, NULL, &hints, &infoptr); // query DNS server, AAAA record
		if (result != 0 || infoptr == NULL) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
			snprintf(ans, sizeof ans, "NOT_FOUND");
		} else {
			struct sockaddr_in6 *first = (struct sockaddr_in6 *)(infoptr->ai_addr);
			inet_ntop(infoptr->ai_family, &(first->sin6_addr), ans, sizeof ans);
			freeaddrinfo(infoptr);
		}

		printf("resolved address: %s\n", ans);
		fflush(stdout);

		if ((numbytes = sendto(my_fd, ans, strlen(ans), 0, (struct sockaddr *)&client_addr, addr_len)) == -1) {
			perror("sendto");
			return 1;
		}
	}

	close(my_fd);
	return 0;
}
