/********************************************************************
 * HTTP Proxy
 *
 * File:        create_server.h
 * Date:        29 Dec 2011
 * Author:      Xiaomou Wang
 * Email:		wxiaomou@umich.edu
 * Description: This File offer the function create_server HTTP Proxy.
 *********************************************************************/
#ifndef CREATE_SERVER_H_
#define CREATE_SERVER_H_
#include"header.h"
using namespace std;

// take the agvg[1] as portNo to create a server.
int create_server(struct sockaddr_in server_addr, char *port, int argc);

#endif  CREATE_SERVER_H_ 
