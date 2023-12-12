//#pragma once
#ifndef _X_HTTP_H_
#define _X_HTTP_H_
#include "xsocket.h"

class XHttp
{
public:
	XHttp();
	~XHttp();
	XTcp server;
	bool isexit = false;
	bool Start(unsigned short port);
	void Main();
	void Stop();
};

#endif