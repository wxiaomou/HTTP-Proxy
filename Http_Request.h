/******************************************
 * HTTP Proxy
 *
 * File:        Http_Request.h
 * Date:        29 Dec 2011
 * Author:      Xiaomou Wang
 * Email:		wxiaomou@umich.edu
 * Description: Deal with the string of the HTTP request
 ******************************************/
#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_
#include "LogUtils.h"
#include "header.h"
#include "error.h"
#include <string>
class Http_Request {
public:
	// construtor.
	Http_Request();

	// buffer is the request that proxy receive.
	// url is the url we will return by reference.
	// transfer is the valid http request that we 
	// reconstruct.
	status handle_request(char *buffer, char *&url, char *&transfer);

	// method is the method of http request
	// it will creturn what kind of method of 
	// the request is making.
	int handle_method(char *method);

	// paste url.
	char* handle_host(char *&url);

	// check version and repair version
	int handle_version(char *version);

	// access to the absolute url
	char *get_abs_url();

private:
	bool get_head, first_read, request_line;
	char *method;
	char *url;
	char *version;
	char *abs_url;
};

#endif /* HTTP_REQUEST_H_ */
