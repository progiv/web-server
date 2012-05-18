#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "thread.hpp"
#include "server.hpp"
using namespace std;
#define BACK_LOG 5
void *client(void *arg){
	//pthread_detach();//nonrequired as it was explitly declared before
	Thread *t = (Thread*)arg;
	t->receive();
	t->respond();
	delete t;
	pthread_exit(NULL);
	return NULL;
}

Server::Server(int port){
	ls = socket(AF_INET, SOCK_STREAM, 0);
	if (ls == -1) throw "Socket making error";
	int optval = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (0 != bind(ls, (struct sockaddr *) &addr, sizeof(addr)))
		throw "Socket binding error";
	if (-1 == listen(ls, BACK_LOG))
		throw "Socket listen error";
	std::cout << "Listening at "<< port << std::endl;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
}

Server::~Server(){
	pthread_attr_destroy(&attr);
	shutdown(ls, SHUT_RDWR);
	close(ls);
}
	
void Server::work(){
	Thread *thread;
	for (;;){
		thread = new Thread;
		socklen_t addrlen = sizeof(thread->addr);
		if (-1 == (thread->fd = ::accept(ls, (struct sockaddr *) &(thread->addr), &addrlen)))
			throw "accept error";
		std::cout << thread->addr.sin_addr.s_addr << ' ' << ntohs(thread->addr.sin_port) << std::endl;
		pthread_attr_init(&attr);
		pthread_create(&thread->t, &attr, client, thread);
	}
}

