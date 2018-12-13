#ifndef QTZ_AUTHENTICATIONSOURCE_HPP
#define QTZ_AUTHENTICATIONSOURCE_HPP

#include <QString>

/*
 * Authentication source is a source of information that provides information
 * on authentication credentials.
*/

class AuthenticationSource {
public:
    AuthenticationSource() = default;
    virtual ~AuthenticationSource() = default;
    virtual QString typeName() const = 0;
    virtual quint32 type() const = 0;
};

#endif // QTZ_AUTHENTICATIONSOURCE_HPP
