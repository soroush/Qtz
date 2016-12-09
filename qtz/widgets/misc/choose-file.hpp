#ifndef QTZ_CHOOSEFILE_HPP
#define QTZ_CHOOSEFILE_HPP

#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QDirModel>
#include <qtz/core/qtz-core.hpp>
#include "../qtz-widgets.hpp"

#ifndef Q_NULLPTR
#if __cplusplus >= 201103L
#define Q_NULLPTR nullptr
#else
#define Q_NULLPTR NULL
#endif
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class ChooseFile;
}
QT_END_NAMESPACE

class QTZ_WIDGETS_SHARED_EXPORT ChooseFile : public QWidget {
    Q_OBJECT

    Q_FLAGS(QFileDialog::Option)
    Q_PROPERTY(QString text READ text WRITE setText USER true DESIGNABLE true STORED
               true)
    Q_PROPERTY(QString filter READ filter WRITE setFilter USER true DESIGNABLE true
               STORED true)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle USER true
               DESIGNABLE true STORED true)
    Q_PROPERTY(QFileDialog::ViewMode viewMode READ viewMode WRITE setViewMode DESIGNABLE true STORED true)
    Q_PROPERTY(QFileDialog::FileMode fileMode READ fileMode WRITE setFileMode DESIGNABLE true STORED true)
    Q_PROPERTY(QFileDialog::AcceptMode acceptMode READ acceptMode WRITE setAcceptMode DESIGNABLE true STORED true)
    Q_PROPERTY(QFileDialog::Options options READ options WRITE setOptions)
    Q_ENUMS(QFileDialog::ViewMode QFileDialog::FileMode QFileDialog::AcceptMode)

public:

    explicit ChooseFile(QWidget *parent = Q_NULLPTR);
    ~ChooseFile();

    QString text() const;
    QString filter() const;
    QString windowTitle() const;

    // Reimplemented QFileDialog API
    QFileDialog::ViewMode viewMode() const;
    QFileDialog::FileMode fileMode() const;
    QFileDialog::AcceptMode acceptMode() const;
    void setOption(QFileDialog::Option option, bool on = true);
    bool testOption(QFileDialog::Option option) const;
    void setOptions(QFileDialog::Options options);
    QFileDialog::Options options() const;

    QLineEdit *lineEdit();

public slots:
    void setText(const QString &text);
    void setFilter(const QString &text);
    void setWindowTitle(const QString &text);
    void chooseFile();
    // Reimplemented QFileDialog API
    void setViewMode(QFileDialog::ViewMode);
    void setFileMode(QFileDialog::FileMode);
    void setAcceptMode(QFileDialog::AcceptMode);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChooseFile *ui;
    QFileDialog* m_dialog;
    QDirModel* m_filesystemModel;
    QCompleter* m_completer;

signals:
    void fileSelected(QString);
};

#endif // QTZ_CHOOSEFILE_HPP
