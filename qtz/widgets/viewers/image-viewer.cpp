#include "image-viewer.hpp"
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <opencv2/core/version.hpp>
#if   (CV_VERSION_EPOCH==2)
#include <opencv2/imgproc/imgproc.hpp>
#elif (CV_VERSION_EPOCH==3)
#include <opencv2/imgproc.hpp>
#endif

ImageViewer::ImageViewer(QWidget* parent) : QGraphicsView(parent)
    , m_fitInView(false)
    , m_preserveMatrix(false)
    , m_zoomFactor(1.05)
    , m_defaultCursor(cursor()) {
    setScene(new QGraphicsScene);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);
    QObject::connect(this, &QWidget::customContextMenuRequested,
                     this, &ImageViewer::showContextMenu);
}

QPixmap ImageViewer::image() const {
    return m_pixmap;
}

bool ImageViewer::preserveMatrix() const {
    return m_preserveMatrix;
}

bool ImageViewer::isFitInView() const {
    return m_fitInView;
}

qreal ImageViewer::zoomFactor() const {
    return m_zoomFactor;
}

qreal ImageViewer::zoom() const {
    return transform().m11();
}

void ImageViewer::setImage(const QString& filePath) {
    m_pixmap = QPixmap(filePath);
    setImage(m_pixmap);
}

void ImageViewer::setImage(const QPixmap& image) {
    QGraphicsPixmapItem* item =
        new QGraphicsPixmapItem(image);
    // Preserve transformation matrix
    QTransform previousTransform;
    if(m_preserveMatrix) {
        previousTransform = transform();
    }
    resetMatrix();
    scene()->clear();
    scene()->setSceneRect(0, 0, image.width(), image.height());
    scene()->addItem(item);
    if(m_fitInView) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    } else if(m_preserveMatrix){
        // Restore transformation matrix
        setTransform(previousTransform);
    } else {
        resetMatrix();
    }
}

void ImageViewer::setImage(const cv::Mat& image) {
    if(image.empty()) {
        resetMatrix();
        scene()->clear();
        return;
    }
    std::vector<uchar> buffer;
    cv::imencode(".jpeg", image, buffer);
    if(buffer.size() != 0) {
        if(m_pixmap.loadFromData(buffer.data(), buffer.size(), "JPEG")) {
            setImage(m_pixmap);
        }
    }
}

void ImageViewer::setPreserveMatrix(bool preserve) {
    if(m_preserveMatrix == preserve) {
        return;
    }
    m_preserveMatrix = preserve;
    emit preserveMatrixChanged(m_preserveMatrix);
}

void ImageViewer::setFitInView(bool fit) {
    if(fit == m_fitInView) {
        return;
    }
    m_fitInView = fit;
    if(fit) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    } else {
        resetMatrix();
    }
    emit fitInViewChanged(m_fitInView);
}

void ImageViewer::setZoomFactor(qreal factor) {
    if(factor == m_zoomFactor) {
        return;
    }
    qreal abs = ::abs(factor);
    if(abs > 1.5) {
        m_zoomFactor = 1.5;
    } else if(abs < 1.01) {
        m_zoomFactor = 1.01;
    }
    emit zoomFactorChanged(m_zoomFactor);
}

void ImageViewer::setZoom(qreal factor) {
    QTransform matrix = QTransform::fromScale(factor, factor);
    setTransform(matrix, false);
}

void ImageViewer::showContextMenu(const QPoint& position) {
    QMenu contextMenu(tr("Image Viewer Options"), this);

    QAction* resetZoom = new QAction("Reset Zoom", this);
    QAction* fit = new QAction("Fit In View", this);
    fit->setCheckable(true);
    fit->setChecked(m_fitInView);
    connect(resetZoom, &QAction::triggered,
            this, &QGraphicsView::resetMatrix);
    connect(fit, &QAction::toggled,
            this, &ImageViewer::setFitInView);
    contextMenu.addAction(fit);
    contextMenu.addSeparator();
    contextMenu.addAction(resetZoom);
    // Zoom Levels
    QMenu zoomLevels(tr("Zoom Level"), this);
    zoomLevels.addSection(QString("Zoom: %1%").arg(zoom() * 100));
    QList<int> levels;
    levels << 10 << 20 << 30 << 40 << 50
           << 60 << 70 << 80 << 90 << 100
           << 150 << 200 << 400;
    for(int i : levels) {
        QAction* level = new QAction(QString("%1%").arg(i), this);
        connect(level, &QAction::triggered,
                std::bind(&ImageViewer::setZoom, this,
                          static_cast<qreal>(i) / 100.0));
        zoomLevels.addAction(level);
    }
    contextMenu.addMenu(&zoomLevels);
    contextMenu.exec(mapToGlobal(position));
}

void ImageViewer::resizeEvent(QResizeEvent* e) {
    if(m_fitInView) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    QGraphicsView::resizeEvent(e);
}

void ImageViewer::wheelEvent(QWheelEvent* e) {
    if(e->delta() != 0 && !m_fitInView) {
        qreal scaleFactor = e->delta() > 0 ? m_zoomFactor : 1.0 / m_zoomFactor;
        scale(scaleFactor, scaleFactor);
    }
    e->accept();
}
