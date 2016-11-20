#ifndef QTZ_DATA_HPP
#define QTZ_DATA_HPP
#include <QtCore/qglobal.h>

#if defined(QTZ_DATA_LIBRARY)
#  define QTZ_DATA_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_DATA_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTZ_DATA_HPP
