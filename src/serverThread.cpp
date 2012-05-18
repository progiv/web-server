/**
 * The main file, initilizes server
 * First argument - is a path to workspace.
 */
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "server.hpp"

int main(int argc, char ** argv){
	if (argc >= 1){
		if (!((0 == access(argv[1], X_OK | R_OK)) && (0 == chdir(argv[1]))))
			perror(argv[1]);
		char buf[1000];
		if (NULL != getcwd(buf,1000));
			std::cout <<"Working from:"<< std::endl << buf << std::endl;
	}
	Server server(40000);
	server.work();
	std::cout << "End of working - something is going strange." << std::endl;
	return 0;
}
