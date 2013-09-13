#include "data/wizard-create-database.h"
#include "ui_wizard-create-database.h"

WizardCreateDatabase::WizardCreateDatabase(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardCreateDatabase),
    m_sqlVisibility(false) {
    ui->setupUi(this);
}

WizardCreateDatabase::~WizardCreateDatabase() {
    delete ui;
}

bool WizardCreateDatabase::SqlVisibility() {
    return this->m_sqlVisibility;
}

void WizardCreateDatabase::setSqlVisibility(bool visibility) {
    this->m_sqlVisibility = visibility;
}

void WizardCreateDatabase::changeEvent(QEvent *e) {
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int WizardCreateDatabase::nextId() const {
    switch (this->currentId()) {
    case (int)Page::Intro:
        return (int)Page::Configure;
    case (int)Page::Configure:
        if(configurationCheck()) {
            return (int)Page::Confirm;
        }
        else {
            return (int)Page::Configure;
        }
        break;
    default:
        break;
    }
}

bool WizardCreateDatabase::configurationCheck() const {
}
