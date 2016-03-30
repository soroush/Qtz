#ifndef WIDGETS_H
#define WIDGETS_H
#include <QtCore/qglobal.h>

#if defined(QTZ_WIDGETS_LIBRARY)
#  define QTZ_WIDGETS_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTZ_WIDGETS_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WIDGETS_H
