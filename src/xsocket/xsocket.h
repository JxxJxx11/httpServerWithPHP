//#pragma once
//��֪��Ϊʲô����"#pragma once"  xhttp�����ļ�������
//һֱ��"error C2011"����

//֮ǰ xsocket.h XHttp.h XHttpNet.h XHttpRes.h ȫ������ "#pragma once"  xhttp�����ļ��ű���ɹ�,��֪������ɶ  Ȼ�����Щ"#pragma once"��ȥ���˾�������

//�ҵ�ԭ����  ������֮ǰ�İ汾
//��� xsocket.h ԭ��Ϊ�˷�ֹ�ض��忪ͷ�����
//#ifndef _X_SOCKET_H_
//#define _X_SCOKET_H_
//ifndef �� define �����ݲ�ͬ
//����xsocket.h XHttp.h XHttpNet.h XHttpRes.h ȫ������ "#pragma once" ȫ�������˻�����
//���ֻҪ��ifndef �� define �����ݸĳ���ͬ����������
//��
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