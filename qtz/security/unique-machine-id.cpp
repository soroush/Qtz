#include "unique-machine-id.hpp"
#include <QCryptographicHash>

QByteArray& operator<<(QByteArray& l, quint8 r) {
    l.append(r);
    return l;
}

QByteArray& operator<<(QByteArray& l, quint16 r) {
    return l<<quint8(r>>8)<<quint8(r);
}

QByteArray& operator<<(QByteArray& l, quint32 r) {
    return l<<quint16(r>>16)<<quint16(r);
}

#ifdef Q_OS_WIN
// UniqueMachineID
QString UniqueMachineID::getMachineID() {
    m_machineID.clear();
    appendMacHash();
    appendVolumeHash();
    appendCpuHash();
    appendMachineName();
    QByteArray hash = QCryptographicHash::hash(m_machineID,QCryptographicHash::Sha3_512);
    return QString::fromUtf8(hash.toHex());
}

quint16 UniqueMachineID::hashMacAddress(PIP_ADAPTER_INFO info) {
    quint16 hash = 0;
    for(quint32 i = 0; i < info->AddressLength; i++) {
        hash += (info->Address[i] << ((i & 1) * 8));
    }
    return hash;
}

void UniqueMachineID::appendMacHash() {
    quint16 mac1=0;
    quint16 mac2=0;
    IP_ADAPTER_INFO AdapterInfo[32];
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if(dwStatus != ERROR_SUCCESS) {
        return;    // no adapters.
    }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    mac1 = hashMacAddress(pAdapterInfo);
    if(pAdapterInfo->Next) {
        mac2 = hashMacAddress(pAdapterInfo->Next);
    }

    // sort the mac addresses. We don't want to invalidate
    // both macs if they just change order.
    if(mac1 > mac2) {
        quint16 tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
    m_machineID << mac1 << mac2;
}

void UniqueMachineID::appendVolumeHash() {
    DWORD serialNum = 0;
    // Determine if this volume uses an NTFS file system.
    GetVolumeInformation(L"c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
    quint16 hash = (quint16)((serialNum + (serialNum >> 16)) & 0xFFFF);
    m_machineID << hash;
}

void UniqueMachineID::appendCpuHash() {
    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuinfo, 0);
    quint16 hash = 0;
    quint16* ptr = (quint16*)(&cpuinfo[0]);
    for(quint32 i = 0; i < 8; i++) {
        hash += ptr[i];
    }
    m_machineID << hash;
}

void UniqueMachineID::appendMachineName() {
    char computerName[1024];
    DWORD size = 1024;
    GetComputerNameA(computerName, &size);
    QByteArray ba = QByteArray::fromRawData(computerName,::strlen(computerName));
    m_machineID+=ba;
}
#endif
