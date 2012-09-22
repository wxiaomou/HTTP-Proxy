#include"Http_Request.h"
using namespace std;

Http_Request::Http_Request() {
	method = NULL;
	url = NULL;
	abs_url = NULL;
	first_read = true;
	get_head = false;
	request_line = true;
}

// This function analysis and repair the
// HTTP request header and recompose
// HTTP request.

status Http_Request::handle_request(char *buffer, char *&url, char *&transfer) {
	//cout << "buffer = " << buffer << endl;
	int counter = 0;
	bool host;
	char *header = new char[strlen(buffer) * 2];
	transfer = new char[strlen(buffer) * 2];
	char *tmp = buffer;
	int count = 0;
	int num_spac = 0;
	int flag = 0;
	bool relative = false;

	// Check the header of both absolute and relative
	// URL and recompose the header for the request

	while (*tmp != '\0') {
		if (flag < 2) {
			if (*tmp == 'g' && num_spac == 0)
				*tmp = 'G';
			if (*tmp == 'e' && num_spac == 0)
				*tmp = 'E';
			if (*tmp == 't' && num_spac == 0)
				*tmp = 'T';
			if (*tmp != ' ' && num_spac > 1)
				num_spac = 1;
			if (*tmp == ' ' && flag == 0)
				num_spac++;
			if (num_spac > 1) {
				tmp++;
				continue;
			}
			if (*tmp == '/' && num_spac == 1) {
				while (*tmp != ' ') {
					header[count++] = *tmp;
					tmp++;
				}
				relative = true;
				flag = 1;
			}
			if (!relative) {
				if (*tmp == 'H')
					*tmp = 'h';
				if (*tmp == 'T' && num_spac > 0)
					*tmp = 't';
				if (*tmp == 'P' || *tmp == 'p') {
					*tmp = 'p';
					flag = 1;
				}
				if (*tmp == ' ' && flag == 1) {
					flag = 2;
				}
				header[count++] = *tmp;
			}
			if (*tmp == ' ' && flag == 1 && relative) {
				flag = 2;
				header[count++] = *tmp;
				tmp++;
			}
		}
		if (flag == 2) {
			while (*tmp != '\r' && *tmp != '\n') {
				if (*tmp == 'h') {
					*tmp = 'H';
				}
				if (*tmp == 't') {
					*tmp = 'T';
				}
				if (*tmp == 'p' || *tmp == 'P') {
					*tmp = 'P';
					flag = 3;
				}
				if (*tmp != ' ')
					header[count++] = *tmp;
				tmp++;
			}
			header[count - 1] = '0';
			header[count] = *tmp;
			tmp++;
			if (*tmp != '\n')//if request line not terminate with '\n'
				header[count++] = '\n';
			else {
				header[count++] = *tmp;
				tmp++;
			}
		}
		if (flag == 3) {
			header[count] = '\0';
			strcat(header, tmp);
			break;
		}
		tmp++;
	}
	cout << "header = " << header << endl;
	char *p;
	strcpy(transfer, header);
	first_read = false;

	if (request_line) {
		p = strtok(header, " \r\n");
		while (p != NULL) {
			if (strlen(p) == 0)
				continue;
			switch (counter) {
			case 0:
				if (handle_method(p) < 0) {
					printf("invalid method\n");
					return method_not_found;
				}
				counter++;
				break;
			case 1:
				counter++;
				break;
			case 2:
				if (handle_version(p) < 0)
					error("invalid version\n");
				counter++;
				request_line = false;
				break;
			case 3:
				counter++;
				break;
			case 4:
				url = handle_host(p);
				counter++;
				break;
			default:
				break;
			}
			p = strtok(NULL, " \r\n");

		}
	}
	return ok;
}

// Get the absolute url
char *Http_Request::get_abs_url() {
	return this->abs_url;
}

// Handle the method
int Http_Request::handle_method(char *method) {
	char GET[] = "GET";
	if (strcmp(method, GET) == 0) {
		this->method = new char[strlen(method) + 1];
		memcpy(this->method, method, strlen(method));
		return 1;
	} else
		return -1;
}

// parste url from host part.
char* Http_Request::handle_host(char *&url) {
	char *p = url;
	char *result = new char[strlen(p) + 1];
	strcpy(result, url);

	return result;
}

// check if the version is 1.0 if not change to 1.0
int Http_Request::handle_version(char *version) {
	if (strlen(version) > 10)
		return -1;
	if (version[8] == '1')
		version[8] = 0;
	this->version = new char[strlen(version) + 1];
	memcpy(this->version, version, strlen(version));
	return 1;
}
