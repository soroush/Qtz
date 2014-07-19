#ifndef QUERYCOLLECTION_H
#define QUERYCOLLECTION_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QToolButton>
#include <QList>
#include <QPair>
#include <qtz/core/library.h>
#include <qtz/data/database.h>


class QTZ_SHARED_EXPORT QueryCollection : public QWidget
{
    Q_OBJECT
public:
    explicit QueryCollection(QWidget *parent = 0);
    ~QueryCollection();
    void setDescryptorFile(const QString& path);
    void setDescryptor(const QString& content);

signals:

public slots:


private:
    QHBoxLayout* hbox;
    QFormLayout* form;
    QToolButton* addButton;
    QList<QPair<Database::FieldType,QString>> fields;
};

#endif // QUERYCOLLECTION_H
