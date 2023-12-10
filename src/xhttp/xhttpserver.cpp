#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <thread>
#include <regex>
using namespace std;
#include "xsocket.h"

class httpThread {
public:
	void Close(){
		client.Close();
		delete this;
	}

	void Main() {
		char buf[10000] = { 0 };

		for (;;) {
			int recvLen = client.Recv(buf, sizeof(buf) - 1);
			if (recvLen <= 0) {
				Close();
				return;
			}
			buf[recvLen] = '\0';

			printf("========recv========\n%s====================\n", buf);
			string src = buf;
			string pattern = "^([A-Z]+) (.+) HTTP/1";
			regex r(pattern);
			smatch mas;
			regex_search(src, mas, r);
			if (mas.size() == 0) {
				printf("%s failed!\n", pattern.c_str());
				Close();
				return;
			}

			string type = mas[1];
			string path = mas[2];
			if (type != "GET") {
				Close();
				return;
			}
			string filename = path;
			if (path == "/") {
				filename = "/index.html";
			}
			string filepath = "www";
			filepath += filename;
			FILE* fp = fopen(filepath.c_str(), "rb");
			if (fp == NULL) {
				Close();
				return;
			}
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			printf("file size is %d\n", filesize);

			string rmsg = "";
			rmsg = "HTTP/1.1 200 OK\r\n";
			rmsg += "Server: XHttp\r\n";
			rmsg += "Content-Type: text/html\r\n";
			rmsg += "Content-Length: ";
			char bsize[128] = { 0 };
			sprintf(bsize, "%d", filesize);
			rmsg += bsize;
			rmsg += "\r\n";
			//rmsg += "10\r\n";
			rmsg += "\r\n";
			//rmsg += "0123456789";
			//发送消息头
			int sendSize = client.Send(rmsg.c_str(), rmsg.size());
			printf("sendSize = %d\n", sendSize);
			printf("========send========\n%s\n====================\n", rmsg.c_str());
			//发送正文
			for (;;) {
				int len = fread(buf, 1, sizeof(buf), fp);
				if (len <= 0) break;
				int re = client.Send(buf, len);
				if (re <= 0) break;
			}
		}
		
		Close();
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
		httpThread* th = new httpThread;
		th->client = client;
		std::thread sth(&httpThread::Main, th);
		sth.detach();
	}
	char ch = getchar();
	server.Close();
	return 0;
}