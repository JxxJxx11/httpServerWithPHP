//#pragma once
#ifndef _X_HTTPRES_H_
#define _X_HTTPRES_H_

#include "xsocket.h"
#include <string>
class XHttpRes
{
public:
	XHttpRes();
	~XHttpRes();
	bool SetRequest(std::string request);
	std::string GetHead();
	int Read(char* buf, int bufsize);
private:
	int filesize;
	FILE* fp;
};

#endif