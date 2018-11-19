#include <qtz/widgets/viewers/image-viewer.hpp>
#include "image-viewer_plugin.hpp"

#include <QtCore/QtPlugin>

ImageViewerPlugin::ImageViewerPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.imageViewerPlugin")
}

void ImageViewerPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool ImageViewerPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* ImageViewerPlugin::createWidget(QWidget* parent) {
    return new ImageViewer(parent);
}

QString ImageViewerPlugin::name() const {
    return QLatin1String("ImageViewer");
}

QString ImageViewerPlugin::group() const {
    return QLatin1String("Qtz Views");
}

QIcon ImageViewerPlugin::icon() const {
    return QIcon::fromTheme("photo",
                            QIcon{":/qtz_plugin/images/compass.png"});
}

QString ImageViewerPlugin::toolTip() const {
    return QLatin1String("Imabe Viewer Widget");
}

QString ImageViewerPlugin::whatsThis() const {
    return QLatin1String("This widget implements an image box capable of viewing cv::Mat, QPixmap and QImage");
}

bool ImageViewerPlugin::isContainer() const {
    return false;
}

QString ImageViewerPlugin::domXml() const {
    return QLatin1String("<widget class=\"ImageViewer\" name=\"imageViewer\">\n</widget>\n");
}

QString ImageViewerPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/viewers/image-viewer.hpp");
}
