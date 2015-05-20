#include "C_SOCKET.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define MAX_CONNECTION 3

C_SOCKET::C_SOCKET():
  m_sock_server_fd(-1)
  ,m_sock_client_fd(-1)
{}

C_SOCKET::~C_SOCKET()
{}

bool C_SOCKET::Init( void )
{
  bool l_status = true;

  m_sock_server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if ( -1 == m_sock_server_fd )
  {
    l_status = false;
  }
  else
  {
    m_server_socket.sin_addr.s_addr = INADDR_ANY; 
    m_server_socket.sin_family = AF_INET;
    m_server_socket.sin_port = htons(PORT);
  }

  return l_status;
}

bool C_SOCKET::Bind( void )
{
  bool l_status = true;

  if ( bind(m_sock_server_fd, (struct sockaddr*)&m_server_socket, sizeof(m_server_socket)) < 0 )
  {
    l_status = false;
  }

  return l_status;
}

bool C_SOCKET::Listen( void )
{
  bool l_status = true;

  if ( listen(m_sock_server_fd, MAX_CONNECTION) < 0 )
  {
    l_status = false;
  }

  return l_status;
}

bool C_SOCKET::WaitIncomingConnection ( void )
{
  bool l_status = true;
  int l_size_struct_addr = sizeof(struct sockaddr_in);

  m_sock_client_fd = accept(m_sock_server_fd, (struct sockaddr*)&m_client_socket, (socklen_t*)&l_size_struct_addr);

  if ( m_sock_client_fd  < 0 )
  {
    l_status = false;
  }

  return l_status;
}

void C_SOCKET::Disconnect( void )
{
  close(m_sock_server_fd);
  close(m_sock_client_fd);
}

bool C_SOCKET::Send( string p_message )
{
  bool l_status = true;

  if ( -1 == send(m_sock_client_fd, p_message.data(), p_message.length(), 0) )
  {
    l_status = false;
  }

  return l_status;
}

void C_SOCKET::PrintConnectedClient ( void )
{
  if ( m_sock_client_fd > 0 )
  {
    std::cout << "IP Adress : " << inet_ntoa(m_client_socket.sin_addr) << " Port number : " << ntohs(m_client_socket.sin_port) << std::endl;
  }
}
