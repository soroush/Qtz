#ifndef AUTHPROVIDER_H
#define AUTHPROVIDER_H

#include <QObject>
#include <QMap>
#include "library.h"

QT_BEGIN_NAMESPACE
class QLatin1String;
QT_END_NAMESPACE

/**
  This class provides a mean to simplify password-based authentication.
  Authentication source may be a database, file or network request. Currently only
  database backend is implemented. @ref AuthProvider follows a singleton pattern by design
  and it's permanent by application's lifecycle.
  */
class QTZ_SHARED_EXPORT AuthProvider {
public:
    enum class AuthenticationSource{
        Database,
        File,
        LDAP,
    };
    enum class HashAlgorithm{
        MD5,
        SHA1,
        SHA256,
        SHA512,
    };
private:
    AuthProvider();
    AuthProvider(const AuthProvider&);
    AuthProvider &operator=(const AuthProvider&)=delete;

    static AuthProvider *m_instance;

    QLatin1String authTableName;
    QLatin1String authIDFiled;
    QLatin1String authPassField;
    AuthenticationSource m_source;
    HashAlgorithm m_passwordHash;

public:
    /**
      This method returns a UTF8-encoded string form a hex-encoded encrypted message.
      Encryption schema is same as explained in @ref encryptPassword.
      */
    QString decryptPassword(const QString &);

    /**
      This method returns a hex-encoded, encrypted message. The algorithm used for
      encryption is AES-256 and key is of lenght 32 bytes (256 bit). The key is retrieved
      from command line as a hex-encoded string of length 64. Because each byte is represented
      as two numbers in hex encoding. The key is generated randomly using openssl with this command:
      openssl rand -hex 32
      */
    QString encryptPassword(const QString &);

    /**
    This method hashes given password by message digest algorithm specified in @ref m_passwordHash.
    As like as all other encryption schemas used in Qtz, generated hash is Hex-encoded. So there is no need
    to return a UTF8 string. Duo to keep compatibility with other parts of framework and database, result of
    hash is then encoded to UTF8. Doing this, we will give a penalty of doubled size in storage, but gain
    compatibility and ease of use.
    */
    QString hashPassword(const QString&);
    bool authenticate(const QString &username, const QString &password);
    void initialize(const QLatin1String &_authTableName = QLatin1String("credentials"),
                    const QLatin1String &_authIDFiled = QLatin1String("username"),
                    const QLatin1String &_authPassField = QLatin1String("password"),
                    const AuthenticationSource &_source = AuthenticationSource::Database,
                    const HashAlgorithm &_passwordHash = HashAlgorithm::SHA512);
    static AuthProvider *instance();
};

#endif // AUTHPROVIDER_H
