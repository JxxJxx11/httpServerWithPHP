#include <iostream>
using namespace std;

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define socklen_t int
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#define closesocket close
#endif

int main(int argc, char* argv[]) {
#ifdef WIN32
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif

	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <= 0) {
		cout << "creat socket failed!" << endl;
		return -1;
	}

	//设置为广播类型
	//int opt = 1;
	//setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	//saddr.sin_addr.s_addr = INADDR_BROADCAST;
	saddr.sin_addr.s_addr = htonl(0);
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		cout << "bind port " << port << " failed!" << endl;
		return -2;
	}
	cout << "bind port " << port << " success!" << endl;
	listen(sock, 10);
	sockaddr_in client;
	socklen_t len = sizeof(client);
	char buf[10240] = { 0 };
	int re = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)&client, &len);
	if (re <= 0) {
		cout << "recvfrom failed!" << endl;
		return -3;
	}
	cout << "recv "<< inet_ntoa(client.sin_addr)<< ":" << ntohs(client.sin_port) << endl;

	buf[re] = '\0';
	cout << buf << endl;
	sendto(sock, "67890", 6, 0, (sockaddr*)&client, sizeof(client));
	getchar();
	return 0;
}