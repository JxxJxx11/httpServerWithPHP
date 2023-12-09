#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <thread>

#include "xsocket.h"

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

int main(int argc, char* argv[]) {
	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);

	for (;;) {
		XTcp client = server.Accept();
		TcpThread* th = new TcpThread;
		th->client = client;
		std::thread sth(&TcpThread::Main, th);
		sth.detach();
	}
	char ch = getchar();
	server.Close();
	return 0;
}