#include "auth-provider.h"
#include "qio.h"
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <string>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

QT_USE_NAMESPACE
using namespace std;
using namespace CryptoPP;


AuthProvider *AuthProvider::m_instance = nullptr;

AuthProvider::AuthProvider()
{
}

QString AuthProvider::decryptPassword(const QString &password)
{
    string plain;
    string encrypted = password.toStdString();
    // Hex decode symmetric key:
    HexDecoder decoder;
    decoder.Put( (byte *)PRIVATE_KEY,32*2 );
    decoder.MessageEnd();
    word64 size = decoder.MaxRetrievable();
    char *decodedKey = new char[size];
    decoder.Get((byte *)decodedKey, size);
    // Generate Cipher, Key, and CBC
    byte key[ AES::MAX_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(decodedKey), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::MAX_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    try {
        CBC_Mode<AES>::Decryption Decryptor
        ( key, sizeof(key), iv );
        StringSource( encrypted, true,
                      new HexDecoder(new StreamTransformationFilter( Decryptor,
                                     new StringSink( plain ) ) ) );
    }
    catch (Exception &e) {
        // TODO: Try a better one
        //return e.what();
    }
    catch (...) {
        // TODO: Try a better one
        //return "Unknown Error";
    }
    return QString::fromStdString(plain);
}

QString AuthProvider::encryptPassword(const QString &password)
{
    string plain = password.toStdString();
    string ciphertext;
    // Hex decode symmetric key:
    HexDecoder decoder;
    decoder.Put( (byte *)PRIVATE_KEY, 32*2 );
    decoder.MessageEnd();
    word64 size = decoder.MaxRetrievable();
    char *decodedKey = new char[size];
    decoder.Get((byte *)decodedKey, size);
    // Generate Cipher, Key, and CBC
    byte key[ AES::MAX_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(decodedKey), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::MAX_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    CBC_Mode<AES>::Encryption Encryptor( key, sizeof(key), iv );
    StringSource( plain, true, new StreamTransformationFilter( Encryptor,
                  new HexEncoder(new StringSink( ciphertext ) ) ) );
    return QString::fromStdString(ciphertext);
}

QString AuthProvider::hashPassword(const QString &password)
{
    std::string hashedPassword;
    switch(this->m_passwordHash) {
    case HashAlgorithm::MD5: {
            CryptoPP::Weak1::MD5 hasher;
            CryptoPP::StringSource source(password.toUtf8().data(), true,
                                          new CryptoPP::HashFilter(hasher,
                                                  new CryptoPP::HexEncoder (
                                                          new CryptoPP::StringSink(hashedPassword))));
            break;
        }
    case HashAlgorithm::SHA1: {
            CryptoPP::SHA1 hasher;
            CryptoPP::StringSource source(password.toUtf8().data(), true,
                                          new CryptoPP::HashFilter(hasher,
                                                  new CryptoPP::HexEncoder (
                                                          new CryptoPP::StringSink(hashedPassword))));
            break;
        }
    case HashAlgorithm::SHA256: {
            CryptoPP::SHA256 hasher;
            CryptoPP::StringSource source(password.toUtf8().data(), true,
                                          new CryptoPP::HashFilter(hasher,
                                                  new CryptoPP::HexEncoder (
                                                          new CryptoPP::StringSink(hashedPassword))));
            break;
        }
    case HashAlgorithm::SHA512: {
            CryptoPP::SHA512 hasher;
            CryptoPP::StringSource source(password.toUtf8().data(), true,
                                          new CryptoPP::HashFilter(hasher,
                                                  new CryptoPP::HexEncoder (
                                                          new CryptoPP::StringSink(hashedPassword))));
            break;
        }
    }
    return QString::fromStdString(hashedPassword); // Convert from ASCII to UTF8.
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
