#pragma once
#include "Prerequisites.h"
#include "NetworkHelper.h"
#include "CryptoHelper.h"

class
Server {
public:
  Server() = default;
  Server(int port); // Constructor that initializes the server with a specific port

  ~Server();

  /*
  * @brief Starts the server and listens for incoming connections.
  * @return true If the server starts successfully.
  * @return false If an error occurs while starting the server.
  */
  bool
  Start();

  /*
  * @brief Stops the server and cleans up resources.
  */
  void
  WaitForClient();

  /*
  * @brief Receives an encrypted message from the client.
  * This function will block until a message is received.
  * It will decrypt the message and print it to the console.
  */
  void
  ReceiveEncryptedMessage();

  /*
  * @brief Starts the receive loop in a separate thread.
  * This function will continuously receive messages from the client
  * and print them to the console.
  * It will run until the server is stopped.
  */
  void
  StartReceiveLoop();

  /*
  * @brief Sends an encrypted message to the client.
  * It will encrypt the message before sending it.
  */
  void
  SendEncryptedMessageLoop();

  /*
  * @brief Starts the chat loop where the server can send and receive messages.
  */
  void
  StartChatLoop();

private:
  int m_port;
  SOCKET m_clientSock;
  NetworkHelper m_net;
  CryptoHelper m_crpto;
  std::thread m_rxThread; // Thread for receiving messages
  std::atomic<bool> m_running{ false }; // Flag to control the running state of the server
};