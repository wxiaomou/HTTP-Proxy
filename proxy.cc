/******************************************
 * HTTP Proxy
 *
 * File:        proxy.cc
 * Date:        29 Dec 2011
 * Author:      Xiaomou Wang
 * Email:		wxiaomou@umich.edu
 * Description: Description: This File contains the main funchtions of a HTTP Proxy.
 *****************************************/

#include <string>
#include <pthread.h>
#include <iostream>
#include "header.h"
#include "create_server.h"
#include "Http_Request.h"
#include "LogUtils.h"
using namespace std;

void *handle_request(void *arg);
int send_request_to_remote(char *str, char* transfer) throw (int);
void read_and_send_back(int connfd, int transfer_sock, char *buffer);
char *get_ip(char *url, int connfd);
inline void check_method(status req_return, int connfd) throw (int);

// main function to receive request 
// then create teh thread to deal
// with the reuqest. 

int main(int argc, char *argv[]) {
	if (argc < 2)
		error("Invalid input");
	int sockfd, n;
	socklen_t len;
	struct sockaddr_in server_addr;
	sockfd = create_server(server_addr, argv[1], argc);
	pthread_t tmp;

	//wait for client requests. 
	while (true) {
		try {
			int *connfd = new int;
			char * begin;
			unsigned int name_length = 0;
			struct sockaddr_in client_addr;
			len = sizeof(client_addr);

			//to get the next completed connection 
			//from the front of the completed connection queue.
			*connfd = accept(sockfd, (struct sockaddr*) &client_addr, &len);
			if (*connfd < 0)
				error("Error in accept");

			//create a thread to handle the request.
			pthread_create(&tmp, NULL, &handle_request, (void *) connfd);
		} catch (bad_alloc) {
			printf("mem allocate fail\n");
		}
	}
	return 0;
}

// This function will receive the request,
// analysis the request fix the request,
// transfer the request and receive the 
// response.
// arg here will be the socket if between
// browser and proxy.

void *handle_request(void *arg) {
	pthread_t threadNo = pthread_self();
	pthread_detach(pthread_self());
	log_d("wxiaomou", string("thead created No.") + to_string(threadNo)
			+ string("\n"));
	int connfd = *((int*) arg);
	char* transfer, *url;
	Http_Request req;
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);

	try {
		//read the message sent by the client.
		read(connfd, buffer, BUFFER_SIZE);

		//deal with the request.
		status req_return = req.handle_request(buffer, url, transfer);
		check_method(req_return, connfd);

		char *str;
		str = get_ip(url, connfd);

		// transfer the request to remote server.
		int transfer_sock = send_request_to_remote(str, transfer);

		// Read data from server and send back to browser.
		read_and_send_back(connfd, transfer_sock, buffer);

		delete[] str;
		delete[] url;
		log_d("wxiaomou", string("Thread No ") + to_string(threadNo) + string(
				" end"));
	} catch (int value) {
		log_d("wxiaomou", string("Thread No ") + to_string(threadNo) + string(
				" terminated ") + to_string(value));
	} catch (bad_alloc) {
		log_d("wxiaomou", string("Thread No ") + to_string(threadNo) + string(
				" terminated by bad_alloc"));
	}
	close(connfd);
}

// This function will transfer the requet 
// from the browser to the server.
// Return the socket that connect the proxy
// and the remotw server.
// str is the ip address for the server.
// transfer the the http request. 

int send_request_to_remote(char *str, char* transfer) throw (int) {
	struct sockaddr_in orgin_addr;
	int n = 0;
	int portNo = 80;

	//Build a socket.
	int transfer_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (transfer_sock < 0)
		error("Error opening socket");

	//Initialize server_address.
	bzero((char *) &orgin_addr, sizeof(orgin_addr));
	orgin_addr.sin_family = AF_INET;
	orgin_addr.sin_port = htons(portNo);

	//Connect the client to the server.
	if (inet_pton(AF_INET, str, &orgin_addr.sin_addr) != 1)
		error("error in pton\n");
	if (connect(transfer_sock, (sockaddr *) &orgin_addr, sizeof(orgin_addr))
			< 0) {
		log_d("wxiaomou", string("Error in connect"));
		throw 1;
	}

	//send the text message to the server.
	n = write1(transfer_sock, transfer, strlen(transfer));

	if (n < 0)
		throw 2;
	return transfer_sock;
}

// read data from remote service and send back to browser.
// connfd is the socket id between proxy and browser.
// transfer_sock is the socket the connect to the remote 
// server.
void read_and_send_back(int connfd, int transfer_sock, char *buffer) {
	int n = 0;
	bzero(buffer, BUFFER_SIZE);
	while ((n = read1(transfer_sock, buffer, BUFFER_SIZE)) > 0) {
		write1(connfd, buffer, n);
		bzero(buffer, BUFFER_SIZE);
	}
}

// Get the ip address.
// connfd is the socket id.
char *get_ip(char *url, int connfd) {
	char *name = new char[strlen(url) + 1];
	char *str = new char[INET_ADDRSTRLEN];
	strcpy(name, url);
	//printf("url is %s thread No %d\n", name, threadNo);

	struct hostent *host_addr = get_ipaddr(name, connfd);
	if (host_addr == NULL)
		throw 0;
	inet_ntop(host_addr->h_addrtype, host_addr->h_addr, str, INET_ADDRSTRLEN);
	printf("ip is %s\n", str);
	delete[] name;
	return str;
}

// check if method other than 'GET'.
// req_return is the status that given from http_handle_request.
// connfd is the socket id.
inline void check_method(status req_return, int connfd) throw (int) {
	if (req_return == method_not_found) {
		char invalid[] = "HTTP/1.0 501 Not Implemented\r\n";
		//send error message back.
		if (write1(connfd, invalid, strlen(invalid)) < 0)
			error("ERROR sending 501 response");
		throw 3;
	}
}
