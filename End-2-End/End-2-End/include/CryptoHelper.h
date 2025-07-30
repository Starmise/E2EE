#pragma once
#include "Prerequisites.h"
#include "openssl/rsa.h"
#include "openssl/aes.h"

class
  CryptoHelper {
public:
  CryptoHelper();
  ~CryptoHelper();

  // RSA
  /*
  * @brief
  */
  void
  GenerateRSAKeys();

  /*
  * @brief
  */
  std::string
  GetPublicKeyString() const;

  /*
  * @brief
  */
  void
  LoadPeerPublickey(const std::string& pemkey);

  // AES
  /*
  * @brief
  */
  void
  GenerateAESKey();

  /*
  * @brief
  */
  std::vector<unsigned char>
  EncryptAESKeyWithPeer();

  /*
  * @brief
  */
  void
  DecryptAESKey(const std::vector<unsigned char> encryptedikey);

  /*
  * @brief
  */
  std::vector<unsigned char>
  AESEncrypt(const std::string& plaintext, std::vector<unsigned char> outIV);

  /*
  * @brief
  */
  std::string
  AESDecrypt(const std::vector<unsigned char>& ciphertext,
             const std::vector<unsigned char>& iv);

private:
  RSA* rsaKeyPair; // Own pair of keys
  RSA* peerPublicKey; // Peer public key
  unsigned char aesKey[32]; // AES-256 Key
};