#include "dialog-user-login.h"
#include "ui_dialog-user-login.h"

#include <qtz/core/auth-provider.h>

#include <QDebug>

DialogUserLogin::DialogUserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUserLogin),
    tries(0)
{
    ui->setupUi(this);
    createConnections();
}

DialogUserLogin::~DialogUserLogin()
{
    delete ui;
}

void DialogUserLogin::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogUserLogin::synchChecks()
{
    if(! (ui->checkBoxRememberPassword->isChecked() && ui->checkBoxRememberUsername->isChecked()))
        ui->checkBoxDontShow->setChecked(false);
}

void DialogUserLogin::login()
{
    ++tries;
    if(tries < 5)
    {
        qDebug() << 4;
        if(AuthProvider::instance()->checkLogin(ui->lineEditUsername->text(),
                                                ui->lineEditPassword->text()))
        {
            // TODO: generate role
            accept();
        }
        else
        {
            ui->labelStatus->setText(tr("Error: Incorrect username and/or password!\n"
                                        "You have %1 %2 left. "
                                        "If you forgot your password, please contact your data administrator.")
                                     .arg(5-tries).arg((tries==1?"try":"tries")));
        }
    }
    else
    {
        ui->labelStatus->setText(tr("You have reachem maximux number of allowed trie. For security reasons"
                                    "access through this program is denied.\n"
                                    "For more information contact your data administrator."));
        ui->labelUsername->setEnabled(false);
        ui->labelPassword->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->pushButtonLogin->setEnabled(false);
        ui->checkBoxDontShow->setEnabled(false);
        ui->checkBoxRememberPassword->setEnabled(false);
        ui->checkBoxRememberUsername->setEnabled(false);
    }
}

void DialogUserLogin::createConnections()
{
    connect(ui->pushButtonLogin,SIGNAL(clicked()),this,SLOT(login()));
}

