#ifndef QTZ_UNIQUEMACHINEID_H
#define QTZ_UNIQUEMACHINEID_H

#include "qtz-security.hpp"

#include <QByteArray>

class QTZ_SECURITY_SHARED_EXPORT UniqueMachineID {
public:
    static QByteArray getMachineID(const QByteArray& salt=QByteArray());
};

#endif // QTZ_UNIQUEMACHINEID_H
