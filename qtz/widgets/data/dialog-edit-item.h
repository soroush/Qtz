#ifndef DIALOGEDITITEM_H
#define DIALOGEDITITEM_H

#include <QDialog>
#include "qtz-widgets.h"

class QTZ_WIDGETS_SHARED_EXPORT DialogEditItem : public QDialog {
    Q_OBJECT
public:
    explicit DialogEditItem(QWidget *parent = 0);

signals:

public slots:

private:
    QObject editingObject;

};

#endif // DIALOGEDITITEM_H
