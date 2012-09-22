/******************************************
 * HTTP Proxy
 *
 * File:        header.h
 * Date:        29 Dec 2011
 * Author:      Xiaomou Wang
 * Email:		wxiaomou@umich.edu
 * Description: contains required header files, constant definitions and auxiliary functions
 *****************************************/

#ifndef http_sock_header
#define http_sock_header
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <typeinfo>
#include <iostream>
using namespace std;

// Use this buffer size whenever 
// you want to open a buffer to store data.
const int BUFFER_SIZE = 1000000;
// The server should allow up to
// 10 concurrent connections.
const int MAX_CONCURRENT = 10;

// log error.
void error(char *msg);

// read from socket.
ssize_t read1(int fd, void *vptr, size_t n) throw (int);

// write to a socket.
ssize_t write1(int fd, void *vptr, size_t n) throw (int);

// get the address struct.
struct hostent* get_ipaddr(char *name, int connfd);
#endif http_sock_header
