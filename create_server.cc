#include"create_server.h"
using namespace std;

int create_server(struct sockaddr_in server_addr, char *port, int argc) {
	int sockfd, portNo;
	//Build a TCP socket.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	unsigned int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (sockfd < 0)
		error("Error in opening socket");
	//initialize the server address.
	bzero((char *) &server_addr, sizeof(server_addr));
	portNo = atoi(port);
	server_addr.sin_family = AF_INET;
	//change the order of portNo to the network order from host order.
	server_addr.sin_port = htons(portNo);
	//assign a local IP address to the server.
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//assigns a local protocol address to a socket.
	if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
		error("Error in binding");
	listen(sockfd, MAX_CONCURRENT);
	return sockfd;
}
