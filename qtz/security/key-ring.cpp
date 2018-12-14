#include "key-ring.hpp"

#include <QString>

QString KeyRing::provideKey() const {
    return QString::fromLatin1(provideRawKey().toHex());
}

QString KeyRing::provideIV() const {
    return QString::fromLatin1(provideRawIV().toHex());
}

KeyRing* KeyRing::defaultKeyRing() {
    static KeyRing* defaultKeyRingInstance = new DefaultKeyRing();
    return defaultKeyRingInstance;
}

QByteArray DefaultKeyRing::provideRawIV() const {
    return QByteArray::fromHex(QString("22351acd67403bf27c1b3279df6fbfe8")
                               .toLatin1());
}

QByteArray DefaultKeyRing::provideRawKey() const {
    return QByteArray::fromHex(QString("7a14792d110a74c1383b387914c5f47e")
                               .toLatin1());
}

size_t DefaultKeyRing::keySize() const {
    return 16;
}
