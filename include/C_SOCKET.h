#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#define ADDR "127.0.0.1"
#define PORT 5005

using namespace std;

class C_SOCKET
{
  public:
    C_SOCKET();
    virtual ~C_SOCKET();
    bool Init ( void );
    bool Connect ( void );
    void Disconnect ( void );
    bool Send ( string p_message );

  private:
    int m_sock_fd;
    struct sockaddr_in m_server_socket;
};
