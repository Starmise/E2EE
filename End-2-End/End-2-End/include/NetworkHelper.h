#pragma once
#include "Prerequisites.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class
NetworkHelper {
public:
  /*
  * @brief Constructor and Destructor
  */
  NetworkHelper();
  ~NetworkHelper();

  // Server mode
  /*
  * @brief Starts a server socket on the specified port and leaves it in listening mode.
  * @param port TCP port to be used to listen for incoming connections.
  * @return true If the server starts successfully.
  * @return false If an error occurs in any step.
  */
  bool
  StartServer(int port);

  /*
  * @brief Wait for and accept an incoming client.
  * @return SOCKET Socket of the accepted client, or INVALID_SOCKET if it fails.
  */
  SOCKET
  AcceptClient();

  // Client mode
  /*
  * @brief Connects to the server specified by IP and port.
  * @param ip Server IP address.
  * @param port Server port.
  * @return true If the connection was successful.
  * @return false If the connection failed.
  */
  bool 
  ConnectToServer(const std::string& ip, int port);

  // Send and receive data
  /*
  * @brief Sends a text string through the socket.
  * @param socket The socket to send data through.
  * @param data The string to be sent.
  */
  bool
  SendData(SOCKET socket, const std::string& data);

  /*
  * @brief Sends binary data (e.g., AES, RSA) through the socket.
  * @param socket The socket to send data through.
  * @param data The data to be sent.
  */
  bool
  SendData(SOCKET socket, const std::vector<unsigned char>& data);

  /*
  * @brief Receive a text string from the socket.
  * @param socket The socket to receive data from.
  * @return The received string, or an empty string if an error occurs.
  */
  std::string
  ReceiveData(SOCKET socket);

  /*
  * @brief Receives binary data (e.g., AES, RSA) from the socket.
  * @param socket The socket to receive data from.
  * @param size The number of bytes to receive.
  * @return A vector containing the received data.
  */
  std::vector<unsigned char>
  ReceiveDataBinary(SOCKET socket, int size = 0);

  /*
  * @brief Closes the socket, but doen't check if the socket is valid.
  * @param socket The socket to be closed.
  */
  void
  close(SOCKET socket);

  /*
  * @brief
  */
  bool
  SendAll(SOCKET s, const unsigned char* data, int len);

  /*
  * @brief
  */
  bool
  ReceiveExact(SOCKET s, unsigned char* out, int len);

  SOCKET m_serverSocket = -1; // Server socket
private:
  bool m_initialized; // Indicates if the network helper has been initialized
};