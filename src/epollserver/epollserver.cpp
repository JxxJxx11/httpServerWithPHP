#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>

#include "xsocket.h"

#ifdef WIN32

class TcpThread {
public:
	void Main() {
		char buffer[1024] = { 0 };
		for (;;) {
			int len = client.Recv(buffer, sizeof(buffer) - 1);
			if (len <= 0 || strstr(buffer, "quit") != NULL) {
				char re[] = "quit success!\n";
				printf("client close!\n");
				client.Send(re, strlen(re) + 1);
				client.Close();
				break;
			}
			buffer[len] = 0;
			printf("recv: %s", buffer);
			client.Send("ok\n", 4);
		}
		delete this;
}
	XTcp client;
};

#else

#include <sys/epoll.h>

#endif

int main(int argc, char* argv[]) {
	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);
	server.SetBlock(false);
#ifdef WIN32
	for (;;) {
		XTcp client = server.Accept();
		TcpThread* th = new TcpThread;
		th->client = client;
		std::thread sth(&TcpThread::Main, th);
		sth.detach();
	}
#else
	//创建epoll
	int epfd = epoll_create(256);
	//注册事件
	epoll_event ev;
	ev.data.fd = server.sock;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);
	epoll_event events[20];
	char buf[1024] = { 0 };
	char* mes = "HTTP/1.1 200 OK\r\nContent-Length:1\r\n\nX";
	int size = strlen(mes);
	for (;;) {
		int count = epoll_wait(epfd, events, 20, 500);
		if (count <= 0) continue;
		for (int i = 0; i < count; i++) {
			if (events[i].data.fd == server.sock) {
				for (;;) {
					XTcp client = server.Accept();
					if (client.sock <= 0) {
						break;
					}
						ev.data.fd = client.sock;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
				}
			}
			else {
				XTcp client;
				client.sock = events[i].data.fd;
				client.Recv(buf, 1024);
				client.Send(mes, size);
				epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);
				client.Close();
			}
		}
	}
#endif
	
	char ch = getchar();
	server.Close();
	return 0;
}