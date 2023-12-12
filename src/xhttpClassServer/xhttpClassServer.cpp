#include <stdlib.h>
#include <stdio.h>

#include "XHttp.h"
#include "xsocket.h"
#include <signal.h>
using namespace std;

int main(int argc, char* argv[]) {
#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif
	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}
	XHttp server;
	server.Start(port);

	getchar();
	return 0;
}