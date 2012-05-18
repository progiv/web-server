#ifndef SERVER_HPP
#define SERVER_HPP
#include <pthread.h>
class Server{
	int ls;
	pthread_attr_t attr;
public:
	Server(int port = 40000);
	~Server();
	void work();
};
#endif //SERVER_HPP
