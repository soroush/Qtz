#include "license-manager.hpp"
#include "crypto.hpp"
#include "unique-machine-id.hpp"

#include <QDebug>
#include <QCryptographicHash>

LicenseManager::LicenseManager(const QByteArray& publicKeyPEM,
                               const QByteArray& saltMD5,
                               const QByteArray& softwareCodeMD5,
                               const QByteArray& customerCodeMD5,
                               const QByteArray& activationCodeRaw):
    m_publicKey{publicKeyPEM},
    m_saltMD5{saltMD5},
    m_softwareCodeMD5{softwareCodeMD5},
    m_customerCodeMD5{customerCodeMD5},
    m_activationCode{activationCodeRaw} {
}

bool LicenseManager::checkActivationCode() const {
    const QByteArray sn = serialNumber();
    return Crypto::verifyRaw(sn, m_activationCode, m_publicKey);
}

void LicenseManager::setSalt(const QByteArray& salt) {
    m_saltMD5 = salt;
}

void LicenseManager::setSoftwareCode(const QByteArray& sc) {
    m_softwareCodeMD5 = sc;
}

void LicenseManager::setCustomerCode(const QByteArray& cc) {
    m_customerCodeMD5 = cc;
}

void LicenseManager::setActivationCode(const QByteArray& ac) {
    m_activationCode = ac;
}

void LicenseManager::setPublicKeyFromPEM(const QByteArray& publicKey) {
    m_publicKey = publicKey;
}

QByteArray LicenseManager::serialNumber() const {
    QByteArray sn =
        QCryptographicHash::hash(UniqueMachineID::getMachineID(m_saltMD5),
                                 QCryptographicHash::Md5);
    if(sn.size() != m_softwareCodeMD5.size()) {
        qDebug() << "MD5 Sizes mismatch!";
    }
    for(int index = 0; index < sn.size(); ++index) {
        sn[index] = sn[index] ^ m_softwareCodeMD5[index];
        sn[index] = sn[index] ^ m_customerCodeMD5[index];
    }
    return sn;

}

