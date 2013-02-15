#ifndef AGT_GLOBAL_H
#define AGT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AGT_LIBRARY)
#  define AGTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define AGTSHARED_EXPORT
#endif

#ifndef DEBUG
#define DEBUG
#endif

#endif // AGT_GLOBAL_H
