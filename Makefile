CC=g++
CFLAGS= -w

all: proxy 


proxy: proxy.o header.o http_request.o create_server.o
	$(CC) $(CFLAGS) -o proxy proxy.o header.o http_request.o create_server.o -lpthread
proxy.o:proxy.cc
	$(CC) $(CFLAGS) -o proxy.o -c proxy.cc -lpthread
header.o: header.cc header.h
	$(CC) $(CFLAGS) -o header.o -c header.cc
http_request.o: http_request.h http_request.cc
	$(CC) $(CFLAGS) -o http_request.o -c http_request.cc
create_server.o: create_server.cc create_server.h
	$(CC) $(CFLAGS) -o create_server.o -c create_server.cc

clean:
	rm -f proxy *.o

