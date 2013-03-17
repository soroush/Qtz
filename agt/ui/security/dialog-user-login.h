#ifndef DIALOGUSERLOGIN_H
#define DIALOGUSERLOGIN_H

#include <QDialog>

namespace Ui {
class DialogUserLogin;
}

class DialogUserLogin : public QDialog
{
    Q_OBJECT

private:
    void createConnections();
    
public:
    explicit DialogUserLogin(QWidget *parent = 0);
    ~DialogUserLogin();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::DialogUserLogin *ui;
    quint8 tries;

private slots:
    void synchChecks();
    void login();
};

#endif // DIALOGUSERLOGIN_H
