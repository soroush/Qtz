#ifndef QTZ_SECURITY_HPP
#define QTZ_SECURITY_HPP
#include <QtCore/qglobal.h>

#if defined(QTZ_SECURITY_LIBRARY)
#  define QTZ_SECURITY_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_SECURITY_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTZ_SECURITY_HPP
