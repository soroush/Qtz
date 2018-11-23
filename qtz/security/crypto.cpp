#include "crypto.hpp"
#include <string.h>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <openssl/aes.h>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>

#define Q(x) #x
#define QUOTE(x) Q(x)

std::string string_to_hex(const unsigned char* input, size_t len) {
    static const char* const lut = "0123456789ABCDEF";
    std::string output;
    output.reserve(2 * len);
    for(size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


void hex_to_string(unsigned char* output, const unsigned char* input, size_t size) {
    static const unsigned char* const lut = (const unsigned char*)("0123456789ABCDEF");
    for(size_t i = 0; i < size; i += 2) {
        char a = input[i];
        const unsigned char* p = std::lower_bound(lut, lut + 16, a);
        if(*p != a) {
            throw std::invalid_argument("not a hex digit");
        }
        char b = input[i + 1];
        const unsigned char* q = std::lower_bound(lut, lut + 16, b);
        if(*q != b) {
            throw std::invalid_argument("not a hex digit");
        }
        output[i / 2] = (((p - lut) << 4) | (q - lut));
    }
}

Crypto::Crypto() {
}

QString Crypto::decrypt(const QString& input) {
    if(input.isEmpty()) {
        return "";
    }
    std::string cypher_hex_str = input.toStdString();
    const size_t size = cypher_hex_str.length();
    const size_t length = size / 2;
    unsigned char* cypher_hex = (unsigned char*)(cypher_hex_str.c_str());
    unsigned char* cypher_data = new unsigned char[length];
    unsigned char* plain_data = new unsigned char[length];
    hex_to_string(cypher_data, cypher_hex, size);
    const unsigned char pk[AES_BLOCK_SIZE] = QTZ_PRIVATE_KEY;
    AES_KEY key;
    AES_set_decrypt_key(pk, 128, &key);
    unsigned char iv[AES_BLOCK_SIZE] = QTZ_INITIALIZATION_VECTOR;
    AES_cbc_encrypt(cypher_data, plain_data, length, &key, iv, AES_DECRYPT);
    //QString result = QString::fromStdString((char*)(plain_data));
    QString result = QString::fromUtf8((const char*)(plain_data));
    //delete[] cypher_hex;
    //delete[] cypher_data;
    return result;
}

QString Crypto::encrypt(const QString& input) {
    if(input.isEmpty()) {
        return "";
    }
    std::string p_str = input.toStdString();
    size_t size = p_str.size();
    size_t padded_size = size + (AES_BLOCK_SIZE - (size % AES_BLOCK_SIZE));
    unsigned char* plain = new unsigned char[padded_size];
    unsigned char* cypher = new unsigned char[padded_size];
    ::memset(plain, 0x00, padded_size);
    ::memcpy(plain, p_str.c_str(), size);
    AES_KEY key;
    const unsigned char pk[AES_BLOCK_SIZE] = QTZ_PRIVATE_KEY;
    unsigned char iv[AES_BLOCK_SIZE] = QTZ_INITIALIZATION_VECTOR;
    AES_set_encrypt_key(pk, 128, &key);
    AES_cbc_encrypt(plain, cypher, padded_size, &key, iv, AES_ENCRYPT);
    QString result = QString::fromStdString(string_to_hex(cypher, padded_size));
    delete[] plain;
    delete[] cypher;
    return result;
}

QByteArray Crypto::hash(const QByteArray& data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Keccak_512);
}
