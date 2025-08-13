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

bool
Client::Connect() {
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
  auto cipher = m_crypto.AESEncrypt(message, iv);

  // 1) IV (16)
  m_net.SendData(m_serverSock, iv);

  // 2) Size (uint32_t) in network byte order
  uint32_t clen = static_cast<uint32_t>(cipher.size());
  uint32_t nlen = htonl(clen);
  std::vector<unsigned char> len4(reinterpret_cast<unsigned char*>(&nlen),
    reinterpret_cast<unsigned char*>(&nlen) + 4);
  m_net.SendData(m_serverSock, len4);

  // 3) Ciphertext
  m_net.SendData(m_serverSock, cipher);
}

void
Client::SendEncryptedMessageLoop() {
  std::string msg;
  while (true) {
    std::cout << "Client: ";
    std::getline(std::cin, msg);
    if (msg == "/exit") break;

    std::vector<unsigned char> iv;
    auto cipher = m_crypto.AESEncrypt(msg, iv);

    m_net.SendData(m_serverSock, iv);

    uint32_t clen = static_cast<uint32_t>(cipher.size());
    uint32_t nlen = htonl(clen);
    std::vector<unsigned char> len4(reinterpret_cast<unsigned char*>(&nlen),
      reinterpret_cast<unsigned char*>(&nlen) + 4);

    m_net.SendData(m_serverSock, len4);
    m_net.SendData(m_serverSock, cipher);
  }
}

void 
Client::StartReceiveLoop() {
  while (true) {
    // 1) IV (16 bytes)
    auto iv = m_net.ReceiveDataBinary(m_serverSock, 16);
    if (iv.empty()) {
      std::cout << "\n[Client] Connection closed by the server.\n";
      break;
    }

    // 2) Size (4 bytes, network/big-endian)
    auto len4 = m_net.ReceiveDataBinary(m_serverSock, 4);
    if (len4.size() != 4) {
      std::cout << "[Client] Error while receiving size.\n";
      break;
    }
    uint32_t nlen = 0;
    std::memcpy(&nlen, len4.data(), 4);
    uint32_t clen = ntohl(nlen);

    // 3) Ciphertext (clen bytes)
    auto cipher = m_net.ReceiveDataBinary(m_serverSock, static_cast<int>(clen));
    if (cipher.empty()) {
      std::cout << "[Client] Error while receiving data.\n";
      break;
    }

    // 4) Descifrar y mostrar
    std::string plain = m_crypto.AESDecrypt(cipher, iv);
    std::cout << "\n[Server]: " << plain << "\Client: ";
    std::cout.flush();
  }
  std::cout << "[Client] ReceiveLoop ended.\n";
}

void
Client::StartChatLoop() {
  std::thread recvThread([&]() {
    StartReceiveLoop();
    });

  SendEncryptedMessageLoop();

  if (recvThread.joinable())
    recvThread.join();
}
