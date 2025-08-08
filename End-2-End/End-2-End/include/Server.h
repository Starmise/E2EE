#pragma once
#include "Prerequisites.h"
#include "NetworkHelper.h"
#include "CryptoHelper.h"

class
Server {
public:
  Server() = default;
  Server(int port);

  ~Server();

  /*
  */
  bool
  Start();

  /*
  */
  void
  WaitForClient();

  /*
  */
  void
  ReceiveEncryptedMessage();

  /*
  */
  void
  StartReceiveLoop();

  /*
  */
  void
  SendEncryptedMessageLoop();

  /*
  */
  void
  StartChatLoop();

private:
  int m_port;
  SOCKET m_clientSock;
  NetworkHelper m_net;
  CryptoHelper m_crpto;
  std::thread m_rxThread;
  std::atomic<bool> m_running{ false };
};