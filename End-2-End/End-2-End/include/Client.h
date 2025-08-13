#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

class
Client {
public:
  Client() = default;
  Client(const std::string& ip, int port); // Constructor that initializes the client with server IP and port
  ~Client();

  /*
  * @brief Establishes the connection with the server
  * @return true If the connection was successful
  * @return false If the connection was not successful
  */
  bool
  Connect();

  /*
  * @brief Exchange public keys with the server
  */
  void
  ExchangeKeys();

  /*
  * @brief Encrypts the AES key with the server's public key and sends it
  */
  void
  SendAESKeyEncrypted();

  /*
  * @brief Receives an encrypted message from the server, decrypts it, and prints it
  */
  void
  SendEncryptedMessage(const std::string& message);

  /*
  * @brief Receives messages from the server in a loop
  */
  void
  StartReceiveLoop();

  /*
  * @brief Receives messages from the server in a loop, encrypts them, and sends them back.
  */
  void
  SendEncryptedMessageLoop();

  /*
  * @brief Starts the chat loop, which allows the user to send and receive messages
  */
  void
  StartChatLoop();

private:
  std::string m_ip; // The IP address of the server
  int m_port; // The port of the server
  SOCKET m_serverSock; // The socket for the server connection
  NetworkHelper m_net; // Network helper for sending and receiving data
  CryptoHelper m_crypto; // Crypto helper for encryption and decryption
};