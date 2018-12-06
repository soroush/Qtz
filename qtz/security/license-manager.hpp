#ifndef QTZ_SECURITY_LICENSEMANAGER_HPP
#define QTZ_SECURITY_LICENSEMANAGER_HPP

#include <QByteArray>
#include "qtz-security.hpp"

class QTZ_SECURITY_SHARED_EXPORT LicenseManager {
public:
    LicenseManager() = default;
    LicenseManager(const QByteArray& publicKeyPEM,
                   const QByteArray& saltMD5 = QByteArray(),
                   const QByteArray& softwareCodeMD5 = QByteArray(),
                   const QByteArray& customerCodeMD5 = QByteArray(),
                   const QByteArray& activationCodeRaw = QByteArray());
    LicenseManager(const LicenseManager&) = delete;
    ~LicenseManager() = default;

    bool checkActivationCode() const;
    void setSalt(const QByteArray& salt);
    void setSoftwareCode(const QByteArray& sc);
    void setCustomerCode(const QByteArray& cc);
    void setActivationCode(const QByteArray& ac);
    void setPublicKeyFromPEM(const QByteArray& publicKey);

    QByteArray serialNumber() const;

private:
    QByteArray m_publicKey;
    QByteArray m_saltMD5;
    QByteArray m_softwareCodeMD5;
    QByteArray m_customerCodeMD5;
    QByteArray m_activationCode;
};

#endif // QTZ_SECURITY_LICENSEMANAGER_HPP
