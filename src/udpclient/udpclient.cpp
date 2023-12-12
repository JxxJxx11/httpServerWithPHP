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

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int len = sendto(sock, "12345", 6, 0, (sockaddr*)&saddr, sizeof(saddr));
	cout << "sendto size is " << len << endl;
	char buf[1024] = { 0 };
	recvfrom(sock, buf, sizeof(buf) - 1, 0, 0, 0);
	cout << buf << endl;
	getchar();
	return 0;
}