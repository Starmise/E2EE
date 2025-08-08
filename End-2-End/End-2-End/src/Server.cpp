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

void
Server::StartReceiveLoop() {
  while (m_running) {
    // 1) IV (16)
    auto iv = m_net.ReceiveDataBinary(m_clientSock, 16);
    if (iv.empty()) {
      std::cout << "\n[Server] Connection closed by the client.\n";
      break;
    }

    // 2) Size (4 bytes network/big-endian)
    auto len4 = m_net.ReceiveDataBinary(m_clientSock, 4);
    if (len4.size() != 4) {
      std::cout << "[Server] Error while receiving data.\n";
      break;
    }
    uint32_t nlen = 0;
    std::memcpy(&nlen, len4.data(), 4);
    uint32_t clen = ntohl(nlen); // <- Convert the network to host

    // 3) Ciphertext (clen bytes)
    auto cipher = m_net.ReceiveDataBinary(m_clientSock, static_cast<int>(clen));
    if (cipher.empty()) {
      std::cout << "[Server] Error while receiving data.\n";
      break;
    }

    // 4) Decode and show
    std::string plain = m_crpto.AESDecrypt(cipher, iv);
    std::cout << "\n[Client]: " << plain << "\Server: ";
    std::cout.flush();
  }
}

void Server::SendEncryptedMessageLoop() {
  std::string msg;
  while (true) {
    std::cout << "Server: ";
    std::getline(std::cin, msg);
    if (msg == "/exit") break;

    std::vector<unsigned char> iv;
    auto cipher = m_crpto.AESEncrypt(msg, iv);

    // 1) IV (16)
    m_net.SendData(m_clientSock, iv);

    // 2) Size in network order (htonl)
    uint32_t clen = static_cast<uint32_t>(cipher.size());
    uint32_t nlen = htonl(clen);
    std::vector<unsigned char> len4(
      reinterpret_cast<unsigned char*>(&nlen),
      reinterpret_cast<unsigned char*>(&nlen) + 4
    );
    m_net.SendData(m_clientSock, len4);

    // 3) Ciphertext
    m_net.SendData(m_clientSock, cipher);
  }
  std::cout << "[Server] Exiting chat.\n";
}

void
Server::StartChatLoop() {
  std::thread recvThread([&]() {
    StartReceiveLoop();
    });

  SendEncryptedMessageLoop();

  if (recvThread.joinable())
    recvThread.join();
}