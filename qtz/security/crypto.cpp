#include "crypto.hpp"
#include <QCryptographicHash>
#include <QtCrypto>

#include <QDebug>

QByteArray Crypto::decryptRawData(const QByteArray& input,
                                  const QByteArray& rawKey) {
    QCA::SymmetricKey key(rawKey);
    QCA::SecureArray sinput(input);
    QCA::Cipher c("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding,
                  QCA::Decode, key);
    return c.process(sinput).toByteArray();
}

QString Crypto::decrypt(const QString& base64Cipher, const QString& base64Key) {
    QByteArray rawKey = QByteArray::fromBase64(base64Key.toLatin1());
    QByteArray rawCipher = QByteArray::fromBase64(base64Cipher.toLatin1());
    QByteArray rawPlain = decryptRawData(rawCipher, rawKey);
    return QString::fromUtf8(rawPlain);
}

QByteArray Crypto::encryptRawData(const QByteArray& input,
                                  const QByteArray& rawKey) {
    QCA::SymmetricKey key(rawKey);
    QCA::Cipher c("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding,
                  QCA::Encode, key);
    QCA::SecureArray secureInput(input);
    return c.process(secureInput).toByteArray();
}

QString Crypto::encrypt(const QString& input, const QString& base64Key) {
    QByteArray rawKey = QByteArray::fromBase64(base64Key.toLatin1());
    QByteArray base64Cipher = encryptRawData(input.toUtf8(), rawKey).toBase64();
    return QString::fromLatin1(base64Cipher);
}

QByteArray Crypto::signRaw(const QByteArray& input, const QString& PEM,
                           const QString& passphrase) {
    // Read Private Key
    QCA::PrivateKey privKey;
    QCA::ConvertResult convRes;
    QCA::SecureArray passPhrase = passphrase.toUtf8();
    privKey = QCA::PrivateKey::fromPEM(PEM, passPhrase, &convRes);
    if(convRes != QCA::ConvertGood) {
        qWarning() << "Sorry, could not import Private Key";
        return QByteArray{};
    }
    // Sign
    QByteArray signResult = privKey.signMessage(QCA::MemoryRegion(input),
                            QCA::EMSA3_MD5);
    return signResult;


    //    QCA::SecureMessageKey msgKey;
    //    msgKey.setX509PrivateKey(privKey);
    //    QCA::SecureMessageKeyList privKeyList;
    //    privKeyList += msgKey;

    //    QCA::CMS cms;
    //    cms.setPrivateKeys(privKeyList);

    //    QCA::SecureMessage signing(&cms);
    //    signing.setSigner(msgKey);

    //    signing.startSign(QCA::SecureMessage::Detached);
    //    signing.update(input);
    //    signing.end();
    //    signing.waitForFinished();

    //    // check to see if it worked
    //    if(!signing.success()) {
    //        qWarning() << "Error signing: " << signing.errorCode();
    //    }

    //    // get the result
    //    return signing.signature();
}

QString Crypto::sign(const QString& input,
                     const QString& PEM,
                     const QString& passphrase) {
    QByteArray signature = signRaw(input.toUtf8(), PEM, passphrase);
    return QString::fromLatin1(signature.toBase64());
}

bool Crypto::verifyRaw(const QByteArray& message,
                       const QByteArray& signature,
                       const QString& certificate) {
    // Load public key
    QCA::ConvertResult conv_res;
    QCA::PublicKey pub_key = QCA::PublicKey::fromPEM(certificate, &conv_res);
    if(conv_res != QCA::ConvertGood) {
        qDebug() << "Public key could not be loaded";
        return false;
    }
    if(!pub_key.canVerify()) {
        qDebug() << "Bad public key";
        return false;
    } else {
        return pub_key.verifyMessage(QCA::SecureArray(message),
                                     signature, QCA::EMSA3_MD5);
    }
}

bool Crypto::verify(const QString& message,
                    const QString& base64Signature,
                    const QString& certificate) {
    return verifyRaw(message.toUtf8(),
                     QByteArray::fromBase64(base64Signature.toLatin1()),
                     certificate);
}

QByteArray Crypto::hash(const QByteArray& data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Keccak_512);
}
