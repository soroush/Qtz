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
#include <QtCrypto>

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

QByteArray Crypto::decryptRawData(const QByteArray& input, const QByteArray& rawKey) {
    QCA::Initializer init;
    return QCA::Cipher("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Decode,
                       QCA::SymmetricKey(rawKey)).process(QCA::SecureArray(input)).toByteArray();
}

QString Crypto::decrypt(const QString& input, const QString& hexKey) {
    QCA::Initializer init;
    QByteArray rawKey = QByteArray::fromHex(hexKey.toLatin1());
    QByteArray rawInput = QByteArray::fromHex(input.toLatin1());
    QByteArray plainHex = decryptRawData(rawInput, rawKey).toHex();
    QString plainText = QString::fromLatin1(plainHex);
    return plainText;
}

QByteArray Crypto::encryptRawData(const QByteArray& input, const QByteArray& rawKey) {
    return QCA::Cipher("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Encode,
                       QCA::SymmetricKey(rawKey)).process(QCA::SecureArray(input)).toByteArray();
}

QString Crypto::encrypt(const QString& input, const QString& hexKey) {
    QByteArray rawKey = QByteArray::fromHex(hexKey.toLatin1());
    QByteArray rawInput = QByteArray::fromHex(input.toLatin1());
    QByteArray cipherHex = encryptRawData(rawInput, rawKey).toHex();
    QString cipherText = QString::fromLatin1(cipherHex);
    return cipherText;
}

QByteArray Crypto::hash(const QByteArray& data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Keccak_512);
}
