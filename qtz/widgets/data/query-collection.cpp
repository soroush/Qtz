#include "query-collection.h"
#include <QXmlStreamReader>
#include <QFile>

QueryCollection::QueryCollection(QWidget *parent) :
    QWidget(parent)
{
    this->hbox = new QHBoxLayout(this);
    this->form = new QFormLayout(this);
    hbox->addLayout(form,1);
    addButton = new QToolButton(this);
    addButton->setText("");
    addButton->setIconSize(QSize(16,16));
    addButton->setIcon(QIcon(":/images/images/list-add.png"));
    hbox->addWidget(addButton,0);
}

QueryCollection::~QueryCollection()
{
    delete form;
    delete hbox;
}

void QueryCollection::setDescryptorFile(const QString &filename)
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
            if(xml.name() == "FieldSet") {
                valid = true;
            }
            else if(xml.name() == "Field" && valid) {
                QXmlStreamAttributes attrib = xml.attributes();
                QString name = attrib.value("name").toString();
                QString type = attrib.value("type").toString();
                Database::FieldType fieldType = Database::getInstance()->fieldFromString(type);
                this->fields.append(QPair<Database::FieldType,QString>(fieldType,name));
            }
        }
    }
    if(xml.hasError()) {
        // TODO: Handle error
    }
}
