#include "C_SOCKET.h"

#include <arpa/inet.h>
#include <unistd.h>

C_SOCKET::C_SOCKET():
  m_sock_fd(-1)
{}

C_SOCKET::~C_SOCKET()
{}

bool C_SOCKET::Init( void )
{
  bool l_status = true;

  m_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if ( -1 == m_sock_fd )
  {
    l_status = false;
  }
  else
  {
    m_server_socket.sin_addr.s_addr = inet_addr(ADDR);
    m_server_socket.sin_family = AF_INET;
    m_server_socket.sin_port = htons(PORT);
  }

  return l_status;
}

bool C_SOCKET::Connect( void )
{
  bool l_status = true;

  if ( connect(m_sock_fd, (struct sockaddr*)&m_server_socket, sizeof(m_server_socket)) < 0 )
  {
    l_status = false;
  }

  return l_status;
}

void C_SOCKET::Disconnect( void )
{
  close(m_sock_fd);
}

bool C_SOCKET::Send( string p_message )
{
  bool l_status = true;

  if ( -1 == send(m_sock_fd, p_message.data(), p_message.length(), 0) )
  {
    l_status = false;
  }

  return l_status;
}
