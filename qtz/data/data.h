#ifndef DATA_H
#define DATA_H
#include <QtCore/qglobal.h>

#if defined(QTZ_DATA_LIBRARY)
#  define QTZ_DATA_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_DATA_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATA_H
