#include <stdio.h>

#include "xsocket.h"

int main(int argc, char* argv[]) {
	XTcp client;
	client.CreateSocket();
	client.SetBlock(false);
	//这个代码是虚拟机的ip地址
	//在虚拟机上跑client程序出现成功现象
	client.Connect("192.168.239.227", 8080, 3000);
	client.Send("client\n", 7);
	char buf[1024] = { 0 };
	client.Recv(buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}