#include "Prerequisites.h"
#include "Server.h"
#include "Client.h"

/*
 * Simple end-to-end encrypted chat application using RSA and AES for the server.
*/
static void
runServer(int port) {
  Server s(port);
  if (!s.Start()) {
    std::cerr << "[Main] Couldn't start server.\n";
    return;
  }
  s.WaitForClient(); // Key exchange
  s.StartChatLoop(); // Now receive and send in parallel
}

/*
 * Simple end-to-end encrypted chat application using RSA and AE for the client.
*/
static void
runClient(const std::string& ip, int port) {
  Client c(ip, port);
  if (!c.Connect()) { std::cerr << "[Main] Couldn't connect.\n"; return; }

  c.ExchangeKeys();
  c.SendAESKeyEncrypted();

  // Finally, parallel chat
  c.StartChatLoop();
}

int
main(int argc, char** argv) {
  std::string mode, ip;
  int port = 0;

  if (argc >= 2) {
    mode = argv[1];

    if (mode == "server") {
      port = (argc >= 3) ? std::stoi(argv[2]) : 12345;
    }
    else if (mode == "client") {
      if (argc < 4) {
        std::cerr << "Usage: E2EE client <ip> <port>\n";
        return 1;
      }
      ip = argv[2];
      port = std::stoi(argv[3]);
    }
    else {
      std::cerr << "Unknown mode. Use: server | client\n";
      return 1;
    }
  }
  else {
    std::cout << "Mode (server/client): ";
    std::cin >> mode;

    if (mode == "server") {
      std::cout << "Port: ";
      std::cin >> port;
    }
    else if (mode == "client") {
      std::cout << "IP: ";
      std::cin >> ip;
      std::cout << "Port: ";
      std::cin >> port;
    }
    else {
      std::cerr << "Unknown mode. Use: server | client\n";
      return 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  if (mode == "server") runServer(port);
  else runClient(ip, port);

  return 0;
}