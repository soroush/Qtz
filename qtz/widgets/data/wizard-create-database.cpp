#include "wizard-create-database.h"
#include "wizard-page-create-database-intro.h"
#include "wizard-page-create-database-configure.h"
#include "wizard-page-create-database-confirm.h"
#include "wizard-page-create-database-operation.h"
#include <QXmlStreamReader>
#include <QFile>

WizardCreateDatabase::WizardCreateDatabase(QWidget *parent) :
    QWizard(parent),
    m_operationPage(new WizardPageCreateDatabaseOperation(this))
{
    this->setWizardStyle(QWizard::ModernStyle);
    this->addPage(new WizardPageCreateDatabaseIntro(this));
    this->addPage(new WizardPageCreateDatabaseConfigure(this));
    this->addPage(new WizardPageCreateDatabaseConfirm(this));
    this->addPage(m_operationPage);
}

void WizardCreateDatabase::setSqlFile(const QString &filename)
{
    QFile schema(filename);
    if(! schema.open(QFile::ReadOnly)) {
        // TODO: throw exception
        return;
    }
    QXmlStreamReader xml(&schema);
    bool valid = false;
    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "Schema") {
                valid = true;
            }
            else if(xml.name() == "Command" && valid) {
                QXmlStreamAttributes attrib = xml.attributes();
                QString title = attrib.value("title").toString();
                quint8 progress = attrib.value("progress").toString().toUInt();
                QString query = xml.readElementText();
                this->m_operationPage->addSql(query,title,progress);
            }
        }
    }
    if(xml.hasError()) {
        // TODO: Handle error
    }
}

void WizardCreateDatabase::setSqlContent(const QString &filename)
{
    // TODO: Implement
    Q_UNUSED(filename);
}
