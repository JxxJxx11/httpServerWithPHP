#include "XHttp.h"
#include "XHttpNet.h"
#include <thread>
using namespace std;

void XHttp::Stop() {
	isexit = true;
}

void XHttp::Main() {
	while (!isexit) {
		XTcp client = server.Accept();
		if (client.sock <= 0) continue;
		XHttpNet* th = new XHttpNet();
		th->Start(client);
	}
}

bool XHttp::Start(unsigned short port) {
	isexit = false;
	server.CreateSocket();
	if (!server.Bind(port)) return false;
	thread sth(&XHttp::Main, this);
	sth.detach();
	return true;
}

XHttp::XHttp() {

}

XHttp::~XHttp() {

}