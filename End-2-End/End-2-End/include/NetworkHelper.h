#pragma once
#include "Prerequisites.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class
NetworkHelper {
public:
  /*
  * @brief
  */
  NetworkHelper();
  ~NetworkHelper();

  // Server mode
  /*
  * @brief
  */
  bool
  StartServer(int port);

  /*
  * @brief
  */
  int
  AcceptClient();

  // Client mode
  bool 
  ConnectToServer(const std::string& ip, int port);

  // Send and receive data
  /*
  * @brief
  */
  bool
  SendData(SOCKET socket, const std::string& data);

  /*
  * @brief
  */
  bool
  SendData(SOCKET socket, const std::vector<unsigned char>& data);

  /*
  * @brief
  */
  std::string
  ReceiveData(SOCKET socket);

  /*
  * @brief
  */
  std::vector<unsigned char>
  ReceiveData(SOCKET socket, int size = 0);

  /*
  * @brief
  */
  void
  close(SOCKET socket);

private:
  SOCKET m_serverSocket = -1;
  bool m_initialized;
};