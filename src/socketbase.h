#pragma once

// #define _WIN32
#ifdef _WIN32
	#include <winsock.h>

	#define STRUCT
	#define NULL_VALUE NULL
	#define WSAGETLASTERROR WSAGetLastError()
#else
	#include <cstring>
	#include <errno.h>
	#include <netinet/in.h>

	#define SOCKET int
	#define SOCKADDR_IN sockaddr_in
	#define STRUCT struct
	#define NULL_VALUE 0
	#define WSAGETLASTERROR errno
#endif


class SocketBase
{
public:
	SocketBase() : socket_(NULL_VALUE) {
		std::memset(&server_addr_, 0, sizeof(server_addr_));
		std::memset(&client_addr_, 0, sizeof(client_addr_));
	}
	virtual ~SocketBase() {}

	virtual int init(const char* ipaddr, int port);

	virtual int release();

protected:

	SOCKET socket_;

	STRUCT SOCKADDR_IN server_addr_;
	STRUCT SOCKADDR_IN client_addr_;
};

class TCPSocketServerBase : public SocketBase
{
public:
	TCPSocketServerBase() : s_client_(NULL_VALUE) {}

	virtual ~TCPSocketServerBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int release() override;

	virtual int recvData(void* _byte, size_t _len);

private:

	SOCKET s_client_;
};

class TCPSocketClientBase : public SocketBase
{
public:
	TCPSocketClientBase() : precv_buf_(nullptr), buf_size_(1024) {}

	virtual ~TCPSocketClientBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int init(const char* ipaddr, int port, int buf_size);

	virtual int release() override;

	virtual int sendData(void* _byte, size_t _len);

private:

	char* precv_buf_;
	int buf_size_;
};

class UDPSocketServerBase : public SocketBase
{
public:
	UDPSocketServerBase() {}

	virtual ~UDPSocketServerBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int recvData(void* _byte, int _len);
};

class UDPSocketClientBase : public SocketBase
{
public:
	UDPSocketClientBase() {}

	virtual ~UDPSocketClientBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int sendData(void* _byte, size_t _len);
};

