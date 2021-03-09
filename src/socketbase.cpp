// see:https://www.cnblogs.com/curo0119/p/8455464.html
#include "socketbase.h"
#include <stdio.h>


#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
	#define COMMON_CLOSE_SOCKET closesocket
	#define COMMON_CLEANUP WSACleanup();
            auto stat = WSAGetLastError();
	#define COMMON_INVALID_SOCKET INVALID_SOCKET
	#define COMMON_SOCKADDR SOCKADDR
	#define COMMON_INVALID_SOCKET INVALID_SOCKET
	#define CMMON_SOCKLEN_T int
#else
	#include <unistd.h>
	#include <stdlib.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/types.h>

	#define COMMON_CLOSE_SOCKET(x) close(x)
	#define COMMON_SOCKLEN_T socklen_t
	#define COMMON_SOCKADDR sockaddr
	#define COMMON_CLEANUP
	#define COMMON_INVALID_SOCKET -1
	#define COMMON_SOCKET_ERROR -1
#endif


int SocketBase::init(const char* ipaddr, int port)
{
#ifdef _WIN32
	WSADATA wsadata;

	// initialize socket library
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		std::cout << "WSAStartup failed.\n";
		COMMON_CLEANUP;
		return -1;
	}
	std::cout << "WSAStartup success.\n";

	// check if have a new version
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "WSAData version doesn't match.\n";
		COMMON_CLEANUP;
		return -2;
	}
	std::cout << "WSAData version match success.\n";
#endif

	// fill server info
	server_addr_.sin_family = AF_INET;
#ifdef _WIN32
	server_addr_.sin_addr.S_un.S_addr = inet_addr(ipaddr);
#else
	server_addr_.sin_addr.s_addr = inet_addr(ipaddr);
#endif
	server_addr_.sin_port = htons(port);
	return 0;
}

int SocketBase::release()
{
	// close socket
	COMMON_CLOSE_SOCKET(socket_);
	COMMON_CLEANUP;
	return 0;
}

int TCPSocketServerBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);

	// create socket
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == COMMON_INVALID_SOCKET) {
		printf("socket() failed ,Error Code:%d/n", COMMON_INVALID_SOCKET);
		COMMON_CLEANUP;
		return -3;
	}

	if (bind(socket_, (COMMON_SOCKADDR*)&server_addr_, sizeof(COMMON_SOCKADDR)) == COMMON_SOCKET_ERROR) {
		printf("bind failed.\n");
		COMMON_CLEANUP;
		return -4;
	}
	printf("socket bind success.\n");

	// setting socket to listen
	if (listen(socket_, SOMAXCONN) < 0) {
		printf("set listen failed.\n");
		COMMON_CLEANUP;
		return -5;
	}
	printf("set listen success.\nserver listenning ...\n");

	// accept connection request
	COMMON_SOCKLEN_T len = sizeof(COMMON_SOCKADDR);
	s_client_ = accept(socket_, (COMMON_SOCKADDR*)&client_addr_, &len);
	if (s_client_ == COMMON_SOCKET_ERROR) {
		printf("connect failed!\n");
		COMMON_CLEANUP;
		return -6;
	}
	return 0;
}

int TCPSocketServerBase::release()
{
	COMMON_CLOSE_SOCKET(s_client_);
	return SocketBase::release();
}

int TCPSocketServerBase::recvData(void* _byte, size_t _len)
{
	// recv(**接收端**套接字描述符, 存放接收数据的缓冲区, 缓冲区长度, 0)
	// recv函数返回其实际copy的字节数
	// 注意协议接收到的数据可能大于buf的长度，所以
	// 在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完
	if (recv(s_client_, (char*)_byte, (int)_len, 0) < 0) {
		printf("receive failed!\n");
		return -1;
	}
	if (send(s_client_, "received.\n", sizeof("received.\n"), 0) < 0) {
		printf("send failed!\n");
		return -2;
	}
	return 0;
}

int TCPSocketClientBase::init(const char* ipaddr, int port)
{
	precv_buf_ = (char*)malloc(buf_size_);
	SocketBase::init(ipaddr, port);

	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == COMMON_INVALID_SOCKET) {
		printf("socket() failed, Error Code:%d\n", COMMON_INVALID_SOCKET);
		COMMON_CLEANUP;
		return -3;
	}

	if (connect(socket_, (COMMON_SOCKADDR*)&server_addr_, sizeof(COMMON_SOCKADDR)) == COMMON_SOCKET_ERROR) {
		printf("server connect failed!\n");
		COMMON_CLEANUP;
		return -4;
	}
	printf("server connect success!\n");
	return 0;
}

int TCPSocketClientBase::init(const char* ipaddr, int port, int buf_size)
{
	buf_size_ = buf_size;
	return init(ipaddr, port);
}

int TCPSocketClientBase::release()
{
	SocketBase::release();
	free(precv_buf_);
	precv_buf_ = nullptr;
}

int TCPSocketClientBase::sendData(void* _byte, size_t _len)
{
	memset(precv_buf_, 0, buf_size_);
	// send(套接字, 地址, 实际发送的字节数, 0)
	if (send(socket_, (char*)_byte, (int)_len, 0) < 0) {
		printf("send failed!\n");
		return -1;
	}
	if (recv(socket_, precv_buf_, buf_size_, 0) < 0) {
		printf("receive failed!\n");
		return -2;
	}
	printf("receive server message: %s\n", precv_buf_);
	return 0;
}

int UDPSocketServerBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);

	if (bind(socket_, (COMMON_SOCKADDR*)&server_addr_, sizeof(COMMON_SOCKADDR)) == COMMON_SOCKET_ERROR) {
		printf("bind failed!\n");
		COMMON_CLEANUP;
		return -3;
	}
	printf("socket bind success.\n");
	return 0;
}

int UDPSocketServerBase::recvData(void* _byte, int _len)
{
	COMMON_SOCKLEN_T clientaddr_len = sizeof(client_addr_);
	if (recvfrom(socket_, (char*)_byte, _len, 0, (COMMON_SOCKADDR*)&client_addr_, &clientaddr_len) == COMMON_SOCKET_ERROR) {
		printf("receive failed!\n");
		return -1;
	}
	if (sendto(socket_, "received.\n", sizeof("received.\n"),
		0, (COMMON_SOCKADDR*)&client_addr_, _len) < 0) {
			printf("send failed!\n");
			return -2;
		}
	return 0;
}

int UDPSocketClientBase::init(const char* ipaddr, int port)
{
	SocketBase::init(ipaddr, port);
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_ == COMMON_INVALID_SOCKET) {
		printf("socket() failed, Error Code:%d/n", COMMON_INVALID_SOCKET);
		COMMON_CLEANUP;
		return -1;
	}
	return 0;
}

int UDPSocketClientBase::sendData(void* _byte, size_t _len)
{
	if (sendto(socket_, (char*)_byte, (int)_len, 0, (sockaddr*)&server_addr_, sizeof(server_addr_)) == COMMON_SOCKET_ERROR) {
		printf("recvfrom() failed:%d/n", COMMON_INVALID_SOCKET);
		return -1;
	}
	return 0;
}

