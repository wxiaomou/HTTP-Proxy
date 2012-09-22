CC=g++
CFLAGS= -w

all: proxy 


proxy: proxy.o header.o Http_Request.o create_server.o
	$(CC) $(CFLAGS) -o proxy proxy.o header.o Http_Request.o create_server.o -lpthread
proxy.o:proxy.cc
	$(CC) $(CFLAGS) -o proxy.o -c proxy.cc -lpthread
header.o: header.cc header.h
	$(CC) $(CFLAGS) -o header.o -c header.cc
Http_Request.o: Http_Request.h Http_Request.cc
	$(CC) $(CFLAGS) -o Http_Request.o -c Http_Request.cc
create_server.o: create_server.cc create_server.h
	$(CC) $(CFLAGS) -o create_server.o -c create_server.cc

clean:
	rm -f proxy *.o

