#include "auth-provider.h"
#include "qio.h"
#include <string.h>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <openssl/aes.h>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

QT_USE_NAMESPACE

#define Q(x) #x
#define QUOTE(x) Q(x)

std::string string_to_hex(const unsigned char* input, size_t len)
{
    static const char* const lut = "0123456789ABCDEF";
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


void hex_to_string(unsigned char* output, const unsigned char* input, size_t size)
{
    static const unsigned char* const lut = (const unsigned char*)("0123456789ABCDEF");

    for (size_t i = 0; i < size; i += 2)
    {
        char a = input[i];
        const unsigned char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const unsigned char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output[i/2] = (((p - lut) << 4) | (q - lut));
    }
}

AuthProvider *AuthProvider::m_instance = nullptr;

AuthProvider::AuthProvider()
{
}

QString AuthProvider::decryptPassword(const QString &password) {
    std::string cypher_hex_str = password.toStdString();
    size_t size = cypher_hex_str.length();
    unsigned char* cypher_hex = (unsigned char*)(cypher_hex_str.c_str());
    unsigned char* cypher_data = new unsigned char[size/2];
    unsigned char* plain_data = new unsigned char[size/2];
    hex_to_string(cypher_data,cypher_hex,size);
    const unsigned char pk[AES_BLOCK_SIZE] = PRIVATE_KEY;
    AES_KEY key;
    AES_set_decrypt_key(pk,128,&key);
    unsigned char iv[AES_BLOCK_SIZE] = IV;
    AES_ecb_encrypt(cypher_data,plain_data,&key,AES_DECRYPT);
    QString result = QString::fromStdString((char*)(plain_data));
    return result;
}

QString AuthProvider::encryptPassword(const QString &password)
{
    std::string p_str = password.toStdString();
    uint size = p_str.size();
    uint padded_size = size + (16-(size%16));
    unsigned char* plain = new unsigned char[padded_size];
    unsigned char* cypher = new unsigned char[padded_size];
    ::memset(plain,0x00,padded_size);
    ::memcpy(plain,p_str.c_str(),size);
    AES_KEY key;
    const unsigned char pk[AES_BLOCK_SIZE] = PRIVATE_KEY;
    unsigned char iv[AES_BLOCK_SIZE] = IV;
    AES_set_encrypt_key(pk,128,&key);
    AES_ecb_encrypt(plain,cypher,&key,AES_ENCRYPT);
    QString result = QString::fromStdString(string_to_hex(cypher,padded_size));
    delete[] plain;
    delete[] cypher;
    return result;
}

QString AuthProvider::hashPassword(const QString &password)
{
//    std::string hashedPassword;
//    switch(this->m_passwordHash) {
//    case HashAlgorithm::MD5: {
//            CryptoPP::Weak1::MD5 hasher;
//            CryptoPP::StringSource source(password.toUtf8().data(), true,
//                                          new CryptoPP::HashFilter(hasher,
//                                                  new CryptoPP::HexEncoder (
//                                                          new CryptoPP::StringSink(hashedPassword))));
//            break;
//        }
//    case HashAlgorithm::SHA1: {
//            CryptoPP::SHA1 hasher;
//            CryptoPP::StringSource source(password.toUtf8().data(), true,
//                                          new CryptoPP::HashFilter(hasher,
//                                                  new CryptoPP::HexEncoder (
//                                                          new CryptoPP::StringSink(hashedPassword))));
//            break;
//        }
//    case HashAlgorithm::SHA256: {
//            CryptoPP::SHA256 hasher;
//            CryptoPP::StringSource source(password.toUtf8().data(), true,
//                                          new CryptoPP::HashFilter(hasher,
//                                                  new CryptoPP::HexEncoder (
//                                                          new CryptoPP::StringSink(hashedPassword))));
//            break;
//        }
//    case HashAlgorithm::SHA512: {
//            CryptoPP::SHA512 hasher;
//            CryptoPP::StringSource source(password.toUtf8().data(), true,
//                                          new CryptoPP::HashFilter(hasher,
//                                                  new CryptoPP::HexEncoder (
//                                                          new CryptoPP::StringSink(hashedPassword))));
//            break;
//        }
//    }
    return QString(); // Convert from ASCII to UTF8.
}

bool AuthProvider::authenticate(const QString &username,
                                const QString &password)
{
    QSqlQuery loginQuery;
    loginQuery.prepare("SELECT NULL FROM :authTableName WHERE :authIDFiled = :username AND authPassField = :pass LIMINT 1");
    loginQuery.bindValue(":authTableName", this->authTableName);
    loginQuery.bindValue(":authIDFiled", this->authIDFiled);
    loginQuery.bindValue(":authPassField", this->authIDFiled);
    loginQuery.bindValue(":username", username);
    loginQuery.bindValue(":password", hashPassword(password));
    if(loginQuery.exec()) {
        if(loginQuery.next()) {
            return true;
        }
        else {
            QIO::cerr << QObject::tr("Unable to authenticate with provided credentials.") <<
                      endl;
            return false;
        }
    }
    else {
        QIO::cerr << QObject::tr("Unable to execute database query. Reason:") << endl;
        QIO::cerr << loginQuery.lastError().text() << endl;
        return false;
    }
}

void AuthProvider::initialize(const QString &_authTableName,
                              const QString &_authIDFiled, const QString &_authPassField,
                              const AuthenticationSource &_source, const HashAlgorithm &_passwordHash)
{
    this->authTableName = _authTableName;
    this->authIDFiled = _authIDFiled;
    this->authPassField = _authPassField;
    this->m_source = _source;
    this->m_passwordHash = _passwordHash;
}

AuthProvider *AuthProvider::instance()
{
    if(AuthProvider::m_instance == nullptr) {
        m_instance = new AuthProvider();
        m_instance->initialize();
    }
    return m_instance;
}
