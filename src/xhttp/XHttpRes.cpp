#include "XHttpRes.h"

bool XHttpRes::SetRequest(std::string request) {
	return true;
}

std::string XHttpRes::GetHead() {
	return "";
}

int XHttpRes::Read(char* buf, int bufsize){
	return 0;
}

XHttpRes::XHttpRes() {
	
}

XHttpRes::~XHttpRes() {

}