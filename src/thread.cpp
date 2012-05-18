#include <iostream>
#include <string.h>
#include <time.h>
#include "thread.hpp"
#define MAXDATE 200
char RESPOND[] = "./res/respond";
using namespace std;
Thread::Thread(){
	input = "";
}
Thread::~Thread(){
	//std::cout << "Closing client socket" <<std::endl;
	close(this->fd);
}
void Thread::receive(){
	int r;
	do{
		char buf[BUF_SIZE+1];
		r = read(this->fd, buf, BUF_SIZE);
		buf[r] = '\0';
		input += buf;
		if (input[input.size()-1] == '\n' && (input[input.size() - 2] == '\n' || input[input.size() - 3] == '\n' ))
			break;
	}while(r > 0);
	cout << "Got request" << endl << input << endl << "End request" << endl;
}

void Thread::fdProxy(int src, int dst){
	char buf[BUF_SIZE+1], buf1[BUF_SIZE+1];
	string res = "";
	int rd = 0, wr, pos = 0, bufs = 0;
	bool endOfInput = false;
	do{
		//sleep(1);
		if (!endOfInput)
			rd = read(src, buf1 , BUF_SIZE);
		//cout << rd << endl;
		if (rd <= 0){
			rd = 0;
			endOfInput = true;
		}
		buf1[rd] = '\0';
		res += buf1;
		if (pos > BUF_SIZE/2){
			strncpy(buf, buf + pos, bufs - pos);
			bufs = bufs - pos;
			pos = 0;
		}
		int cp = min(BUF_SIZE-bufs, int(res.length()));
		res.copy(buf+bufs, cp, 0);
		res.erase(0, cp);
		bufs += cp;
		wr = send(dst, buf + pos, bufs-pos, MSG_NOSIGNAL);
		pos += wr;
		//cout << "bye " << wr << endl;
	}while (wr > 0);
}

string* Thread::makeHead(){
	string *str = new string();
	*str += "HTTP/1.0\n";
	*str += "Connection: Close\n";
	char stm[MAXDATE];
	time_t tm;
	struct tm gtm;
	time(&tm);
	gmtime_r(&tm, &gtm);
	strftime(stm, MAXDATE, "Date: %a, %d %b %Y %T GMT\n", &gtm);
	*str += stm;
	return str;
}

void Thread::respond(){
	string str;// = "";
	//struct stat st;
	//stat()
	str = *makeHead();
	str += '\n';
	int d = open(RESPOND, O_RDONLY);
	if (d == -1){
		perror(RESPOND);
	}
	send(fd, str.c_str(), str.length(), MSG_NOSIGNAL);
	//TODO split the string for buffer send
	fdProxy(d, fd);
	close(d);
	shutdown(fd, SHUT_RDWR);
}
