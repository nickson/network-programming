//
// multiperson chat server
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(void)
{

	socklen_t addrlen;
	
	struct in_addr my_addr;
	//struct sockaddr_in6 my_sock;
	struct sockaddr_in my_sock;
	fd_set master;    // master file descriptor list
	fd_set read_fds;  // temp file descriptor list for select()

	int fdmax;        // maximum file descriptor number

	int listener;     // listening socket descriptor
	int newfd;        // newly accept()ed socket descriptor
	char buf[256];
	int nbytes;
	
	int yes=1;
	int i, j, rv;

	struct sockaddr_in remoteaddr; // client address


	listener=socket(PF_INET,SOCK_STREAM,0);
	if(listener==-1){
		perror("socket");
		return 1;
	}
	
	memset(&my_sock, 0, sizeof(my_sock));
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	my_sock.sin_family = PF_INET;
	//my_sock.sin_addr.s_addr = inet_addr("192.168.0.2"); //listen on this IP address only
	my_sock.sin_addr.s_addr = INADDR_ANY;	//the server will listen for incoming packets on all of the interfaces (IPv4 addresses)
	//my_sock.sin6_addr = in6addr_any;	//listen on all IPv6 addresses
	my_sock.sin_port = htons(3333); //server PORT number;

	// lose the "address already in use" error message
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	if( bind(listener, (struct sockaddr *)&my_sock, sizeof my_sock) == -1){
		perror("bind");
		return 1;
	}

	if (listen(listener, 10) == -1) {
		perror("listen");
		return 1;
	}
	// add the listener to the master set
	FD_SET(listener, &master);

	// keep track of the biggest file descriptor
	fdmax = listener; // so far, it's this one

	// main loop
	for(;;) {
		read_fds = master; // copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");	//int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); 
			exit(4);
		}

		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				if (i == listener) {
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

					if (newfd == -1) {
						perror("accept");
					} else {
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) {    // keep track of the max
							fdmax = newfd;
						}
						printf("selectserver: new connection from %s on socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
					}
				} else { // if i != listener
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof buf, 0)) > 0) {
						// we got some data from a client
						for(j = 0; j <= fdmax; j++) {
							// send to everyone!
							if (FD_ISSET(j, &master)) {
								// except the listener and ourselves
								if (j != listener && j != i) {
									if (send(j, buf, nbytes, 0) == -1) {
										perror("send");
									}
								}
							}
						}
					} else {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							printf("selectserver: socket %d hung up\n", i);
						} else { // nbytes == -1
							perror("recv");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
					}
				} // END handle data from client
			} // END got new incoming connection
		} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!

	return 0;
}
