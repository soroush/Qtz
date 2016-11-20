#ifndef QTZ_QIO_HPP
#define QTZ_QIO_HPP

#include <QObject>
#include <QTextStream>
#include "qtz-core.hpp"

class QTZ_SHARED_EXPORT QIO : public QObject {
    Q_OBJECT
public:
    explicit QIO(QObject* parent = 0);
    static QTextStream cout;
    static QTextStream cin;
    static QTextStream cerr;
signals:

public slots:


};

#endif // QTZ_QIO_HPP
