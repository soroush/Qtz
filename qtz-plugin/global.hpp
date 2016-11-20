#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTZ_LIBRARY)
#  define QTZ_PLUGIN_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_PLUGIN_SHARED_EXPORT
#endif

#endif // GLOBAL_H
