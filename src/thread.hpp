#ifndef THREAD_HPP
#define THREAD_HPP
#include <pthread.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>

#define BUF_SIZE 1000
extern char RESPOND[];

//using namespace std;
struct Thread{
	int fd;
	sockaddr_in addr;
	std::string input;
	
	Thread();
	~Thread();
	void receive();
	void respond();
	void fdProxy(int src, int dst);
	std::string* makeHead();
};
#endif //THEAD_HPP
