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
#include <sys/wait.h>

#include "thread.hpp"
#include "server.hpp"

using namespace std;
#define BACK_LOG 5

void *client(void *arg){
	Thread *t = (Thread*)arg;
	t->receive();
	t->respond();
	delete t;
	exit(0);
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
}

Server::~Server(){
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
		int pid;
		if (0 == (pid = fork())){
			close(ls);
			client(thread);
			exit(1);//Just to be shure
		} else {
			delete thread;
		}
		waitpid(-1, NULL, WNOHANG);
	}
}

