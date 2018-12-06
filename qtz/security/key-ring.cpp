#include "key-ring.hpp"

#include <QString>

QString KeyRing::provideKey() const {
    return QString::fromLatin1(provideRawKey().toHex());
}

KeyRing* KeyRing::defaultKeyRing() {
    static KeyRing* defaultKeyRingInstance = new DefaultKeyRing();
    return defaultKeyRingInstance;
}

QByteArray DefaultKeyRing::provideRawKey() const {
    return QByteArray::fromHex(QString("7a14792d110a74c1383b387914c5f47e").toLatin1());
}

size_t DefaultKeyRing::keySize() const {
    return 128;
}
