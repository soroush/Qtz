#ifndef SECURITY_H
#define SECURITY_H
#include <QtCore/qglobal.h>

#if defined(QTZ_SECURITY_LIBRARY)
#  define QTZ_SECURITY_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_SECURITY_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SECURITY_H
