#define _CRT_SECURE_NO_WARNINGS

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Xsocket.h"

WSAinit::WSAinit() {
#ifdef WIN32
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif
}

WSAinit::~WSAinit() {
#ifdef WIN32
	WSACleanup();
#endif
}

XSOCKET_API WSAinit wainit;

Xsocket::Xsocket() {

}

Xsocket::~Xsocket() {

}


XTcp::XTcp() {

}

XTcp::~XTcp() {

}

int XTcp::CreateSocket() {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("create socket failed!\n");
		return sock;
	}
	printf("sock: %d create success!\n", sock);
	return sock;
}

bool XTcp::Bind(unsigned short port) {
	if (sock <= 0) {
		CreateSocket();
	}
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		printf("bind port: %d failed!\n", port);
		return false;
	}
	printf("bind port: %d success!\n", port);
	listen(sock, 10);
	return true;
}

XTcp XTcp::Accept() {
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t clen = sizeof(caddr);
	tcp.sock = accept(sock, (sockaddr*)&caddr, &clen);
	if (tcp.sock <= 0) return tcp;
	char* ip = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ip);
	tcp.port = ntohs(caddr.sin_port);
	printf("accept client: %d success! ip: %s port: %d\n", tcp.sock, tcp.ip, tcp.port);
	return tcp;
}

void XTcp::Close() {
	if (sock <= 0) return;
	closesocket(sock);
}

int XTcp::Recv(char* buf, int bufSize) {
	return recv(sock, buf, bufSize, 0);
}

int XTcp::Send(const char* buf, int sendSize) {
	int s = 0;
	while(s != sendSize) {
		int len = send(sock, buf + s, sendSize - s, 0);
		if (len <= 0) break;
		s += len;
	}
	return s;
}

bool XTcp::Connect(const char* ip, unsigned short port, int timeoutms) {
	if (sock <= 0) CreateSocket();
	sockaddr_in  saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	SetBlock(false);
	fd_set set;
	if (connect(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		// printf("2023-12-09:test\n");
		fflush(stdin);
		FD_ZERO(&set);
		FD_SET(sock, &set);
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = timeoutms * 1000;
		if(select(sock + 1, 0, &set, 0, &tm) <= 0) {
			printf("connect timeout or error!\n");
			printf("connect %s: %d failed! : %s\n", ip, port, strerror(errno));
			return false;
		}
	}
	SetBlock(true);
	printf("connect %s: %d success!\n", ip, port);
	return true;
}

bool XTcp::SetBlock(bool isblock) {
	if (sock <= 0) return false;
#ifdef WIN32
	unsigned long ul = 0;
	if(!isblock) ul = 1;
	ioctlsocket(sock, FIONBIO, &ul);
#else
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0)
		return false;
	if (isblock) {
		flags = flags & ~O_NONBLOCK;
	}
	else {
		flags = flags | O_NONBLOCK;
	}
	if (fcntl(sock, F_SETFL, flags) != 0) return false;
#endif
	return true;
}