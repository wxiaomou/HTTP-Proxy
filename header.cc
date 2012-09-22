#include"header.h"
using namespace std;
void error(char *msg) {
	perror(msg);
	exit(0);
}

ssize_t read1(int fd, void *vptr, size_t n) throw (int) {
	size_t nleft;
	ssize_t nread = 0;
	char *ptr;
	ptr = (char *) (vptr);
	nleft = n;
	while (nleft > 0) {
		// if ptr not reach the end of vptr
		// read the massage from the socket.
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;
			else {
				printf("reading error");
				throw 1;
			}
		} else if (nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	// return how many elements have been read.
	return (n - nleft);
}

ssize_t write1(int fd, void *vptr, size_t n) throw (int) {
	size_t nleft;
	ssize_t nwritten;
	char *ptr = (char *) vptr;
	nleft = n;
	while (nleft > 0) {
		// if ptr not reach the end of vptr
		// write the massage to the server.
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else {
				printf("writting error");
				throw 1;
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	// return how many elements have been writen.
	return (n - nleft);
}

struct hostent* get_ipaddr(char *name, int connfd) {
	struct hostent hostbuf, *hp;
	size_t hstbuflen;
	char *tmphstbuf;
	int res;
	int herr;

	hstbuflen = 1024;
	// allocate buffer.
	try {
		tmphstbuf = (char *) malloc(hstbuflen);
		while ((res = gethostbyname_r(name, &hostbuf, tmphstbuf, hstbuflen,
				&hp, &herr)) == ERANGE) {
			// enlarge the buffer.
			hstbuflen *= 2;
			tmphstbuf = (char *) realloc(tmphstbuf, hstbuflen);
		}
	} catch (exception &ex) {
		printf("bad allocation in dns %s\n", ex.what());
	}
	free(tmphstbuf);
	// check for errors.
	if (res || hp == NULL)
		return NULL;
	return hp;
}
