#ifndef UNIQUEMACHINEID_H
#define UNIQUEMACHINEID_H

#include "qtz-security.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <QtGlobal>

#include <QByteArray>
#include <QString>

class QTZ_SECURITY_SHARED_EXPORT UniqueMachineID {
public:
    QString getMachineID();
    UniqueMachineID()=default;
private:
    quint16 hashMacAddress(PIP_ADAPTER_INFO info);
    void appendMacHash();
    void appendVolumeHash();
    void appendCpuHash();
    void appendMachineName();
    QByteArray m_machineID;

};

#endif // UNIQUEMACHINEID_H
