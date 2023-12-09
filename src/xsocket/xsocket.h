#ifndef _X_SOCKET_H_
#define _X_SCOKET_H_

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
extern XSOCKET_API WSAinit wsinit;

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

#endif // !_XSOCKET_H_