// see:https://www.cnblogs.com/curo0119/p/8455464.html
#include "socketbase.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")


int SocketBase::init(const char* ipaddr, int port)
{
	precv_buf_ = new char[BUF_SIZE];
	WSADATA wsadata;

	// initialize socket library
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		std::cout << "WSAStartup failed.\n";
		WSACleanup();
		return -1;
	}
	std::cout << "WSAStartup success.\n";

	// check if have a new version
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "WSAData version doesn't match.\n";
		WSACleanup();
		return -2;
	}
	std::cout << "WSAData version match success.\n";

	// fill server info
	server_addr_.sin_family = AF_INET;
	server_addr_.sin_addr.S_un.S_addr = inet_addr(ipaddr);
	server_addr_.sin_port = htons(port);
	return 0;
}

int SocketBase::close()
{
	delete[] precv_buf_;
	precv_buf_ = nullptr;

	// close socket
	closesocket(socket_);
	WSACleanup();
	return 0;
}

int TCPSocketServerBase::init(const char* ipaddr, int port)
{
	// 调用父类方法对通用变量做初始化
	SocketBase::init(ipaddr, port);

	// create socket
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == INVALID_SOCKET) {
		printf("socket() failed ,Error Code:%d/n", WSAGetLastError());
		WSACleanup();
		return -3;
	}

	if (bind(socket_, (SOCKADDR*)&server_addr_, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		std::cout << "bind failed.\n";
		WSACleanup();
		return -4;
	}
	std::cout << "socket bind success.\n";

	// setting socket to listen
	if (listen(socket_, SOMAXCONN) < 0) {
		std::cout << "set listen failed.\n";
		WSACleanup();
		return -5;
	}
	std::cout << "set listen success.\n";
	std::cout << "server listenning ...\n";

	// accept connection request
	int len = sizeof(SOCKADDR);
	s_client_ = accept(socket_, (SOCKADDR*)&client_addr_, &len);
	if (s_client_ == SOCKET_ERROR) {
		std::cout << "connect failed!\n";
		WSACleanup();
		return -6;
	}
	return 0;
}

int TCPSocketServerBase::recvData(void* _byte, size_t _len)
{
	// recv(**接收端**套接字描述符, 存放接收数据的缓冲区, 缓冲区长度, 0)
	// recv函数返回其实际copy的字节数
	// 注意协议接收到的数据可能大于buf的长度，所以
	// 在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完
	if (recv(s_client_, (char*)_byte, (int)_len, 0) < 0) {
		std::cout << "receive failed!\n";
		return -1;
	}
	if (send(s_client_, "received.\n", sizeof("received.\n"), 0) < 0) {
		std::cout << "send failed!\n";
		return -2;
	}
	return 0;
}

int TCPSocketServerBase::close()
{
	closesocket(s_client_);
	return SocketBase::close();
}

int TCPSocketClientBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);

	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == INVALID_SOCKET) {
		printf("socket() failed, Error Code:%d/n", WSAGetLastError());
		WSACleanup();
		return -3;
	}

	if (connect(socket_, (SOCKADDR*)&server_addr_, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		std::cout << "server connect failed!\n";
		WSACleanup();
		return -4;
	}
	std::cout << "server connect success!\n";
	return 0;
}

int TCPSocketClientBase::sendData(void* _byte, size_t _len)
{
	memset(precv_buf_, 0, BUF_SIZE);
	// send(套接字, 地址, 实际发送的字节数, 0)
	if (send(socket_, (char*)_byte, (int)_len, 0) < 0) {
		std::cout << "send failed!\n";
		return -1;
	}
	if (recv(socket_, precv_buf_, BUF_SIZE, 0) < 0) {
		std::cout << "receive failed!\n";
		return -2;
	}
	std::cout << "receive server message: " << precv_buf_ << "\n";
	return 0;
}

int UDPSocketServerBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);

	if (bind(socket_, (SOCKADDR*)&server_addr_, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		std::cout << "bind failed!\n";
		WSACleanup();
		return -3;
	}
	std::cout << "socket bind success.\n";
	return 0;
}

int UDPSocketServerBase::recvData(void* _byte, int _len)
{
	int clientaddr_len = sizeof(client_addr_);
	if (recvfrom(socket_, (char*)_byte, _len, 0, (SOCKADDR*)&client_addr_, &clientaddr_len) == SOCKET_ERROR) {
		std::cout << "receive failed!\n";
		return -1;
	}
	if (sendto(socket_, "received.\n", sizeof("received.\n"),
		0, (SOCKADDR*)&client_addr_, _len) < 0) {
		std::cout << "send failed!\n";
		return -2;
	}
	return 0;
}

int UDPSocketClientBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_ == INVALID_SOCKET) {
		printf("socket() failed, Error Code:%d/n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	return 0;
}

int UDPSocketClientBase::sendData(void* _byte, size_t _len)
{
	if (sendto(socket_, (char*)_byte, (int)_len, 0, (sockaddr*)&server_addr_, sizeof(server_addr_)) == SOCKET_ERROR) {
		printf("recvfrom() failed:%d/n", WSAGetLastError());
		return -1;
	}
	return 0;
}

