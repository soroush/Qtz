#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <qtz/core/core.h>
#include "qtz-widgets.h"


QT_BEGIN_NAMESPACE
class QVBoxLayout;

namespace Ui {
class EditableLabel;
}
QT_END_NAMESPACE

class TextEditorWindow;

class QTZ_WIDGETS_SHARED_EXPORT EditableLabel : public QWidget {
    Q_OBJECT

public:
    explicit EditableLabel(QWidget *parent = 0);
    ~EditableLabel();

signals:

public slots:
    void startEditig();
    void finishEditig();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditableLabel *ui;
    enum Status_t {Normal = 0, Editing = 1};
    Status_t currentStatus;
    QVBoxLayout *mainLayout;
    QLabel m_label;
    TextEditorWindow *editor;
};

#endif // EDITABLELABEL_H
