#pragma once

// #define _WIN32

#ifdef _WIN32
	#include <winsock.h>
#else

#endif

#define      BUF_SIZE     1024


class SocketBase
{
public:
	SocketBase() : precv_buf_(nullptr), socket_(NULL) {
		memset(&server_addr_, 0, sizeof(server_addr_));
		memset(&client_addr_, 0, sizeof(client_addr_));
	}
	virtual ~SocketBase() {}

	virtual int init(const char* ipaddr, int port);

	virtual int close();

protected:

	char* precv_buf_;

	SOCKET socket_;

	SOCKADDR_IN server_addr_;
	SOCKADDR_IN client_addr_;
};

class TCPSocketServerBase : public SocketBase
{
public:
	TCPSocketServerBase() :s_client_(NULL) {}

	virtual ~TCPSocketServerBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int recvData(void* _byte, size_t _len);

	virtual int close() override;

private:

	SOCKET s_client_;
};

class TCPSocketClientBase : public SocketBase
{
public:
	TCPSocketClientBase() {}

	virtual ~TCPSocketClientBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int sendData(void* _byte, size_t _len);
};

class UDPSocketServerBase : public SocketBase
{
public:
	UDPSocketServerBase() {}

	virtual ~UDPSocketServerBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int recvData(void* _byte, int _len);
};

class UDPSocketClientBase :public SocketBase
{
public:
	UDPSocketClientBase() {}

	virtual ~UDPSocketClientBase() {}

	virtual int init(const char* ipaddr, int port) override;

	virtual int sendData(void* _byte, size_t _len);
};

