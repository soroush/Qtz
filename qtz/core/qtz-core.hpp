#ifndef QTZ_CORE_HPP
    #define QTZ_CORE_H

    #include <QtCore/qglobal.h>

    #if defined(QTZ_CORE_LIBRARY)
        #define QTZ_SHARED_EXPORT Q_DECL_EXPORT
    #else
        #define QTZ_SHARED_EXPORT Q_DECL_IMPORT
    #endif

#endif // QTZ_CORE_HPP
