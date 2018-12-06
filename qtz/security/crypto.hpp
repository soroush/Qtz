#ifndef AUTHPROVIDER_H
#define AUTHPROVIDER_H

#include "qtz-security.hpp"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

/**
  This class provides cryptopgraphy support for all security backend of
  the Qtz project
  */
class QTZ_SECURITY_SHARED_EXPORT Crypto {
private:
    Crypto() = delete;
    ~Crypto() = default;
    Crypto(const Crypto&) = delete;

public:
    static QByteArray decryptRawData(const QByteArray& input,
                                     const QByteArray& rawKey);

    static QString decrypt(const QString& base64Cipher, const QString& hexKey);

    static QByteArray encryptRawData(const QByteArray& input,
                                     const QByteArray& rawKey);

    static QString encrypt(const QString& input, const QString& base64Key);

    static QByteArray signRaw(const QByteArray& input,
                              const QString& PEM,
                              const QString& passphrase);

    static QString sign(const QString& input,
                        const QString& PEM,
                        const QString& passphrase);

    static bool verifyRaw(const QByteArray& message,
                          const QByteArray& signature,
                          const QString &certificate);

    static bool verify(const QString& message,
                       const QString& base64Signature,
                       const QString& certificate);

    /**
     * This method hashes given data by default message digest algorithm.
     * The generated hash is in raw binary format. */
    static QByteArray hash(const QByteArray&);
};

#endif // AUTHPROVIDER_H
