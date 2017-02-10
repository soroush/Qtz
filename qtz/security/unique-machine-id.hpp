#ifndef UNIQUEMACHINEID_H
#define UNIQUEMACHINEID_H

#include "qtz-security.hpp"

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#endif
#include <QtGlobal>

#include <QByteArray>
#include <QString>

class QTZ_SECURITY_SHARED_EXPORT UniqueMachineID {
public:
    QString getMachineID();
    UniqueMachineID()=default;
private:
#ifdef Q_OS_WIN
    quint16 hashMacAddress(PIP_ADAPTER_INFO info);
    void appendMacHash();
    void appendVolumeHash();
    void appendCpuHash();
    void appendMachineName();
#endif
    QByteArray m_machineID;

};

#endif // UNIQUEMACHINEID_H
