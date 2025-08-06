#include "Server.h"

Server::Server(int port) : m_port(port), m_clientSock(-1) {
  // Generate RSA keys when building
  m_crpto.GenerateRSAKeys();
}

Server::~Server() {
  // If it's active, close connection with the client
  if (m_clientSock != -1) {
    m_net.close(m_clientSock);
  }
}

bool
Server::Start() {
  std::cout << "[Server] Starting server on port " << m_port << "...\n";
  return m_net.StartServer(m_port);
}

void
Server::WaitForClient() {
  std::cout << "[Server] Waiting for client connection...\n";

  // Accept incoming connection
  m_clientSock = m_net.AcceptClient();
  if (m_clientSock == INVALID_SOCKET) {
    std::cerr << "[Server] Could not accept client.\n";
    return;
  }
  std::cout << "[Server] Client connected.\n";

  // 1. Send public key to the client server
  std::string serverPubKey = m_crpto.GetPublicKeyString();
  m_net.SendData(m_clientSock, serverPubKey);

  // 2. Receive public key from client
  std::string clientPubKey = m_net.ReceiveData(m_clientSock);
  m_crpto.LoadPeerPublickey(clientPubKey);

  // 3. Receive AES key encrypted with the server's public key
  std::vector<unsigned char> encryptedAESKey = m_net.ReceiveDataBinary(m_clientSock, 256);
  m_crpto.DecryptAESKey(encryptedAESKey);

  std::cout << "[Server] AES key exchanged successfully.\n";
}

void
Server::ReceiveEncryptedMessage() {
  // 1. Receive the IV (Initialization vector)
  std::vector<unsigned char> iv = m_net.ReceiveDataBinary(m_clientSock, 16);

  // 2. Receive encrypted message
  std::vector<unsigned char> encryptedMsg = m_net.ReceiveDataBinary(m_clientSock, 128);

  // 3. Decode message
  std::string msg = m_crpto.AESDecrypt(encryptedMsg, iv);

  // 4. Show message
  std::cout << "[Server] Message received: " << msg << "\n";
}
