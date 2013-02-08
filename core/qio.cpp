#include "qio.h"

QTextStream QIO::qcout;
QTextStream QIO::qcin;
QTextStream QIO::qcerr;

QIO::QIO(QObject *parent) :
    QObject(parent)
{
}
