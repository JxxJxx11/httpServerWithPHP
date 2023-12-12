//#pragma once
//不知道为什么不加"#pragma once"  xhttp工程文件过不了
//一直报"error C2011"错误

//之前 xsocket.h XHttp.h XHttpNet.h XHttpRes.h 全部加了 "#pragma once"  xhttp工程文件才编译成功,不知道改了啥  然后把这些"#pragma once"都去掉了就能跑了

//找到原因了  翻看了之前的版本
//这边 xsocket.h 原来为了防止重定义开头定义的
//#ifndef _X_SOCKET_H_
//#define _X_SCOKET_H_
//ifndef 和 define 的内容不同
//所以xsocket.h XHttp.h XHttpNet.h XHttpRes.h 全部加了 "#pragma once" 全部加上了会有用
//这边只要把ifndef 和 define 的内容改成相同就能运行了
//唉
#ifndef _X_SOCKET_H_
#define _X_SOCKET_H_

#ifdef WIN32
#ifdef XSOCKET_EXPORTS
#define XSOCKET_API __declspec(dllexport)
#else
#define XSOCKET_API __declspec(dllimport)
#endif
#else
#define XSOCKET_API
#endif

class XSOCKET_API WSAinit {
public:
	WSAinit();
	~WSAinit();
};

extern XSOCKET_API WSAinit dwsinit;

class XSOCKET_API Xsocket{
public:
	Xsocket();
	virtual ~Xsocket();
};

class XSOCKET_API XTcp :public Xsocket {
public:
	XTcp();
	~XTcp();
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char* buf, int bufSize);
	int Send(const char* buf, int sendSize);
	bool Connect(const char* ip, unsigned short port, int timeoutms = 1000);
	bool SetBlock(bool isblock);
public:
	int sock = 0;
	unsigned short port = 0;
	char ip[16];
};

class XSOCKET_API XUdp :public Xsocket {
public:
	XUdp();
	~XUdp();
	int CreateSocket();
	bool Bind(unsigned short port);
	void Close();
	int Recv(char* buf, int bufSize);
public:
	int sock = 0;
	unsigned short port = 0;
	char ip[16];
private:
	void* addr = NULL;
};

#endif // !_XSOCKET_H_