#ifndef QTZ_PLUGINS_HPP
#define QTZ_PLUGINS_HPP

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class Qtz : public QObject, public QDesignerCustomWidgetCollectionInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA (IID
                       "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" FILE
                       "qtz_plugin.json")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    explicit Qtz(QObject *parent = 0);

    virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface *> m_widgets;
};

#endif // QTZ_PLUGINS_HPP
