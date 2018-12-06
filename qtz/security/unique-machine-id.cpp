#include "unique-machine-id.hpp"

#include <QFile>

QByteArray UniqueMachineID::getMachineID(const QByteArray& salt) {
    QFile machineIDFile("/etc/machine-id");
    if(!machineIDFile.open(QFile::ReadOnly)) {
        return QByteArray{};
    }
    QByteArray data = QByteArray::fromHex(machineIDFile.readLine());
    machineIDFile.close();
    data.append(salt);
    return data;
}
