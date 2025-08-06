#include "Client.h"

Client::Client(const std::string& ip, int port)
  : m_ip(ip), m_port(port), m_serverSock(INVALID_SOCKET) {
  // Generate RSA key pairs when instantiating
  m_crypto.GenerateRSAKeys();
  // Generates the AES key that will be used to encrypt messages.
  m_crypto.GenerateAESKey();
}

Client::~Client() {
  if (m_serverSock != INVALID_SOCKET) {
    m_net.close(m_serverSock);
  }
}

bool Client::Connect() {
  std::cout << "[Client] Connecting to server " << m_ip << ":" << m_port << "...\n";

  bool connected = m_net.ConnectToServer(m_ip, m_port);
  if (connected) {
    m_serverSock = m_net.m_serverSocket; // Save the socket once connected
    std::cout << "[Client] Connection established.\n";
  }
  else {
    std::cerr << "[Client] Error connecting.\n";
  }

  return connected;
}

void
Client::ExchangeKeys() {
  // 1. Receive the public key of the server
  std::string serverPubKey = m_net.ReceiveData(m_serverSock);
  m_crypto.LoadPeerPublickey(serverPubKey);
  std::cout << "[Client] Public key of the server received.\n";

  // 2. Send the public key of the client
  std::string clientPubKey = m_crypto.GetPublicKeyString();
  m_net.SendData(m_serverSock, clientPubKey);
  std::cout << "[Client] Public key of the client sent.\n";


}

void
Client::SendAESKeyEncrypted() {
  std::vector<unsigned char> encryptedAES = m_crypto.EncryptAESKeyWithPeer();
  m_net.SendData(m_serverSock, encryptedAES);
  std::cout << "[Client] AES key encrypted and sent to server.\n";
}

void
Client::SendEncryptedMessage(const std::string& message) {
  std::vector<unsigned char> iv;
  std::vector<unsigned char> encryptedMsg = m_crypto.AESEncrypt(message, iv);
  
  // Send IV and then the encrypted message
  m_net.SendData(m_serverSock, iv);
  m_net.SendData(m_serverSock, encryptedMsg);
  std::cout << "[Client] Encrypted message sent.\n";
}
