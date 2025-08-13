# E2EE
This repository contains a university project to develop an end-to-end encryption system (2EEE) in C#.

## Features

- End-to-end encryption using RSA (2048-bit) for key exchange and AES (256-bit) for message encryption
- Server-client architecture supporting 1-to-1 chat
- Real-time bidirectional communication
- Command-line interface
- Support for both interactive and command-line argument modes

## Prerequisites

- C++ compiler with C++11 support
- OpenSSL library
- Windows OS (uses Winsock2)

## Building

Compile all source files:
```cpp
Client.cpp
CryptoHelper.cpp
main.cpp
NetworkHelper.cpp
Server.cpp
```

## Usage

The application can be run in either server or client mode:

### Command-line Arguments

Server mode:
```bash
E2EE server [port]
```

Client mode:
```bash
E2EE client <ip> <port>
```

### Interactive Mode

Run the program without arguments and follow the prompts:

1. Choose mode (server/client)
2. For server: Enter port number
3. For client: Enter IP address and port number

### Chat Commands

- Type your message and press Enter to send
- Type `/exit` to quit the chat

## Security Features

- RSA 2048-bit keys for secure key exchange
- AES-256-CBC for message encryption
- Unique IV (Initialization Vector) for each message
- Secure key generation using OpenSSL's RAND_bytes

## Protocol Flow

1. Server starts and waits for connection
2. Client connects to server
3. RSA public key exchange between client and server
4. Client generates AES key and sends it encrypted with server's RSA public key
5. Secure chat begins using AES encryption

## Network Protocol

Messages are sent in the following format:
1. IV (16 bytes)
2. Message length (4 bytes, network byte order)
3. Encrypted message