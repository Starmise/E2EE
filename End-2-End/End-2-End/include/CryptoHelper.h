#pragma once
#include "Prerequisites.h"
#include "openssl/rsa.h"
#include "openssl/aes.h"

class
CryptoHelper {
public:
  // Constructor and Destructor.
  CryptoHelper();
  ~CryptoHelper();

  // RSA
  /*
  * @brief Generate a new pair of 2048-bit RSA keys.
  */
  void
  GenerateRSAKeys();

  /*
  * @brief Returns the public key in PEM format (as a string).
  */
  std::string
  GetPublicKeyString() const;

  /*
  * @brief Loads the peer's public key from a PEM string.
  * @param pemkey The PEM formatted public key string.
  */
  void
  LoadPeerPublickey(const std::string& pemkey);

  // AES
  /*
  * @brief Generate an AES-256 key (32 random bytes).
  */
  void
  GenerateAESKey();

  /*
  * @brief Encrypt the AES key with the peer's public key using RSA.
  */
  std::vector<unsigned char>
  EncryptAESKeyWithPeer();

  /*
  * @brief Decrypt the AES key sent by the client.
  * @param encryptedikey The encrypted AES key received from the client.
  */
  void
  DecryptAESKey(const std::vector<unsigned char>& encryptedikey);

  /*
  * @brief Encrypt a message using AES-256 in CBC mode.
  * @param plaintext The plaintext to be encrypted.
  * @param outIV Vector where the IV used for encryption will be stored.
  * @return The encrypted text as a byte vector.
  */
  std::vector<unsigned char>
  AESEncrypt(const std::string& plaintext, std::vector<unsigned char>& outIV);

  /*
  * @brief Decrypts a message encrypted with AES-256-CBC.
  * @param ciphertext Encrypted vector.
  * @param iv Vector with the IV used during encryption.
  * @return Original text.
  */
  std::string
  AESDecrypt(const std::vector<unsigned char>& ciphertext,
             const std::vector<unsigned char>& iv);

private:
  RSA* rsaKeyPair; // Own pair of keys
  RSA* peerPublicKey; // Peer public key
  unsigned char aesKey[32]; // AES-256 Key
};