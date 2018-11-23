#include "unique-machine-id.hpp"
#include <QCryptographicHash>

#include <QFile>

QByteArray UniqueMachineID::getMachineID(const QByteArray& salt) {
    QFile machineIDFile("/etc/machine-id");
    if(!machineIDFile.open(QFile::ReadOnly)) {
        return QByteArray{};
    }
    QByteArray data = QByteArray::fromHex(machineIDFile.readLine().append(salt));
    machineIDFile.close();
    return QCryptographicHash::hash(data, QCryptographicHash::Md5);
}
