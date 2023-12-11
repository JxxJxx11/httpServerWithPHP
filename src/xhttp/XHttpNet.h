//#pragma once
#ifndef _X_HTTPNET_H_
#define _X_HTTPNET_H_
#include "xsocket.h"
#include "XHttpRes.h"

class XHttpNet
{
public:
	XHttpNet();
	~XHttpNet();
	XTcp client;
	XHttpRes res;
	bool Start(XTcp client);
	void Main();
};

#endif