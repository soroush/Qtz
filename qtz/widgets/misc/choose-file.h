#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H

#include <QWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChooseFile;
}
QT_END_NAMESPACE

class ChooseFile : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText USER true DESIGNABLE true STORED true)
    Q_PROPERTY(QString text READ text WRITE setText USER true DESIGNABLE true STORED true)
    Q_PROPERTY(QString filter READ filter WRITE setFilter USER true DESIGNABLE true STORED true)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle USER true DESIGNABLE true STORED true)
    Q_PROPERTY(QLineEdit* lineEdit READ lineEdit USER true DESIGNABLE true STORED true)

public:
    explicit ChooseFile(QWidget *parent = 0);
    ~ChooseFile();

    void setLabelText(const QString& text);
    QString labelText();

    void setText(const QString& text);
    QString text();
    
    void setFilter(const QString& text);
    QString filter();

    void setWindowTitle(const QString& text);
    QString windowTitle();

    QLineEdit *lineEdit();

public slots:
    void chooseFile();

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::ChooseFile *ui;
    QString m_filter;
    QString m_windowTitle;

signals:
    void fileSelected(QString);
};

#endif // CHOOSEFILE_H