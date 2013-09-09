#ifndef QTZ_H
#define QTZ_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class Qtz : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
#   if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA (IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" FILE "qtz_plugin.json")
#   endif
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
    
public:
    explicit Qtz(QObject *parent = 0);
    
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
    
private:
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};

#endif
