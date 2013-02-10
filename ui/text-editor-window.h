#ifndef TEXTEDITORWINDOW_H
#define TEXTEDITORWINDOW_H

#include <QMainWindow>
#include <agt/global.h>

class QFontComboBox;
class QTextCharFormat;

namespace Ui {
class textEditorWindow;
}

class AGTSHARED_EXPORT TextEditorWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString text READ toHtml WRITE setText NOTIFY textChanged USER true)
    
public:
    explicit TextEditorWindow(QWidget *parent = 0);
    ~TextEditorWindow();
    
    QString toPlainText() const;
    QString toHtml() const;

public slots:
    void setText(QString);

signals:
    void textChanged(QString);

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::textEditorWindow *ui;
    void loadIcons();
    void setupToolbars();
    QFontComboBox *fontCombo;

private slots:
//    void delete_();
//    void find();
//    void replace();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void bold(bool);
    void italic(bool);
    void underline(bool);
    void strickout(bool);
    void changeFont(QFont);
    void updateStatus();
};

#endif // TEXTEDITORWINDOW_H
