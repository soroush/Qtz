#ifndef TEXTEDITORWINDOWPLUGIN_H
#define TEXTEDITORWINDOWPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class TextEditorWindowPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    TextEditorWindowPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);
private:
    bool m_initialized;
};

#endif // TEXTEDITORWINDOWPLUGIN_H
