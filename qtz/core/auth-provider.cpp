#include "auth-provider.h"
#include <crypto++/aes.h>
#include <crypto++/modes.h> // xxx_Mode< >
#include <crypto++/filters.h> // StringSource and StreamTransformation
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <string>
#include <iostream>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

QT_USE_NAMESPACE
using namespace std;
using namespace CryptoPP;

AuthProvider *AuthProvider::m_instance = NULL;
bool AuthProvider::set = false;

QString AuthProvider::decryptPassword(const QString &password) {
    string plain;
    string encrypted = password.toStdString();
    QFile passwordFile(passwordFileName);
    passwordFile.open(QFile::ReadOnly | QFile::Text);
    string pass(passwordFile.readAll().data(), 1024);
    passwordFile.close();
    byte key[ AES::DEFAULT_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(pass.c_str()), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::DEFAULT_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    try {
        CBC_Mode<AES>::Decryption Decryptor( key, sizeof(key), iv );
        StringSource( encrypted, true,
                      new HexDecoder(new StreamTransformationFilter( Decryptor,
                                     new StringSink( plain ) ) ) );
    }
    catch (Exception &e) {
        return e.what();
    }
    catch (...) {
        return "Unknown Error";
    }
    return QString::fromStdString(plain);
}

QString AuthProvider::encryptPassword(const QString &password) {
    string plain = password.toStdString();
    string encrypted;
    QFile passwordFile(passwordFileName);
    passwordFile.open(QFile::ReadOnly | QFile::Text);
    string pass(passwordFile.readAll().data(), 1024);
    passwordFile.close();
    byte key[ AES::DEFAULT_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(pass.c_str()), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::DEFAULT_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    CBC_Mode<AES>::Encryption Encryptor( key, sizeof(key), iv );
    StringSource( plain, true, new StreamTransformationFilter( Encryptor,
                  new HexEncoder(new StringSink( encrypted ) ) ) );
    return QString::fromStdString(encrypted);
}

bool AuthProvider::checkLogin(const QString &username, const QString &password)

{
    QString loginQueryText =
        QString("select null from %1 where %2 = '%3' and %4 = '%5' limit 1")
        .arg(authTableName)
        .arg(authIDFiled)
        .arg(username)
        .arg(authPassField)
        .arg(password);
    QSqlQuery loginQuery;
    loginQuery.prepare(loginQueryText);
    if(loginQuery.exec()) {
        if(loginQuery.next()) {
            return true;
        }
        else {
            wcerr << QObject::tr("Unable to authenticate with provided credentials.").toStdWString()
                  << endl;
            return false;
        }
    }
    else {
        wcerr << QObject::tr("Unable to execute database query. Reason:").toStdWString()
              << endl;
        wcerr << loginQuery.lastError().text().toStdWString();
        return false;
    }
}

void AuthProvider::initialize(const QString &_authTableName,
                              const QString &_authIDFiled,
                              const QString &_authPassField) {
    authTableName = _authTableName;
    authIDFiled = _authIDFiled;
    authPassField = _authPassField;
    set = true;
}

AuthProvider *AuthProvider::instance() {
    if(!set) {
        m_instance = new AuthProvider();
        m_instance->initialize("users", "username", "password");
    }
    return m_instance;
}

void AuthProvider::getData() {
}
