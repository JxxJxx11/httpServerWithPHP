#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <thread>
#include <regex>

#include "xsocket.h"

using namespace std;
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
			string pattern = "^([A-Z]+) /([a-zA-Z0-9]*([.][a-zA-Z0-9]*)?)[?]?(.*)? HTTP/1";
			regex r(pattern);
			smatch mas;
			regex_search(src, mas, r);
			if (mas.size() == 0) {
				printf("%s failed!\n", pattern.c_str());
				Close();
				return;
			}

			string type = mas[1];
			string path = "/";
			path += mas[2];
			string filetype = mas[3];
			string query = mas[4];
			if (filetype.size() > 0) {
				filetype = filetype.substr(1, filetype.size() - 1);
			}
			printf("type: [%s]\n path:[%s]\n filetype:[%s]\n query:[%s]\n", type.c_str(), path.c_str(), filetype.c_str(),query.c_str());
			if (type != "GET") {
				printf("not GET!\n");
				Close();
				return;
			}
			string filename = path;
			if (path == "/") {
				filename = "/index.html";
			}
			string filepath = "www";
			filepath += filename;

			if (filetype == "php") {
				string cmd = "php-cgi ";
				cmd += filepath;
				cmd += " ";
				for (int i = 0; i < query.size(); i++) {
					if (query[i] == '&') query[i] = ' ';
				}
				cmd += query;
				cmd += " > ";
				filepath += ".html";
				cmd += filepath;

				

				printf("%s\n", cmd.c_str());
				system(cmd.c_str());
			}


			FILE* fp = fopen(filepath.c_str(), "rb");
			if (fp == NULL) {
				printf("open file %s failed!\n", filepath.c_str());
				Close();
				return;
			}
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			printf("file size is %d\n", filesize);
			if (filetype == "php") {
				char c = 0;
				int headsize = 0;
				while (fread(&c, 1, 1, fp) > 0) {
					headsize++;
					if (c == '\r') {
						fseek(fp, 3, SEEK_CUR);
						headsize += 3;
						break;
					}
				}
				filesize = filesize - headsize;
			}
			

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
			printf("========end========\n\n\n");
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