#ifndef DIALOGEDITTABLE_H
#define DIALOGEDITTABLE_H

#include <QDialog>

namespace Ui {
class DialogEditTable;
}

class DialogEditTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogEditTable(QWidget *parent = 0);
    ~DialogEditTable();
    
protected:
    void changeEvent(QEvent *e);

protected slots:
    virtual void insert()=0;
    virtual void remove()=0;
    virtual void edit()=0;
    virtual void refresh()=0;
    virtual void revert()=0;
    virtual void save()=0;
    
private:
    Ui::DialogEditTable *ui;
};

#endif // DIALOGEDITTABLE_H
