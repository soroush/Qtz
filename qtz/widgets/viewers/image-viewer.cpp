#include "image-viewer.hpp"
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QDebug>
#include <opencv2/core/version.hpp>
#if   (CV_VERSION_EPOCH==2)
#include <opencv2/imgproc/imgproc.hpp>
#elif (CV_VERSION_EPOCH==3)
#include <opencv2/imgproc.hpp>
#endif

ImageViewer::ImageViewer(QWidget* parent) : QGraphicsView(parent)
    ,m_fitInView(false)
    ,m_isMoving(false)
    ,m_zoomFactor(1.05)
    ,m_defaultCursor(cursor()) {
    setScene(new QGraphicsScene);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

QPixmap ImageViewer::image() const {
    return m_pixmap;
}

bool ImageViewer::isFitInView() const {
    return m_fitInView;
}

qreal ImageViewer::zoomFactor() const {
    return m_zoomFactor;
}

void ImageViewer::setImage(const QString& filePath) {
    m_pixmap = QPixmap(filePath);
    setImage(m_pixmap);
}

void ImageViewer::setImage(const QPixmap& image) {
    QGraphicsPixmapItem* item =
        new QGraphicsPixmapItem(image);
    resetMatrix();
    scene()->clear();
    scene()->setSceneRect(0,0,image.width(),image.height());
    scene()->addItem(item);
}

void ImageViewer::setImage(const cv::Mat& image) {
    cv::Mat img = image.clone();
    //    cv::cvtColor(img,img,CV_BGR2RGB);
    QImage im((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_BGR30);
    if(!im.isNull()) {
        m_pixmap =QPixmap::fromImage(im);
        setImage(m_pixmap);
    }
}

void ImageViewer::setFitInView(bool fit) {
    this->m_fitInView = fit;
    if(fit) {
        fitInView(scene()->sceneRect(),Qt::KeepAspectRatio);
    } else {
        resetMatrix();
    }
}

void ImageViewer::setZoomFactor(qreal factor) {
    qreal abs = ::abs(factor);
    if(abs>1.5) {
        m_zoomFactor=1.5;
    } else if(abs<1.01) {
        m_zoomFactor=1.01;
    }
}

void ImageViewer::resizeEvent(QResizeEvent* e) {
    if(m_fitInView) {
        fitInView(scene()->sceneRect(),Qt::KeepAspectRatio);
    }
    QGraphicsView::resizeEvent(e);
}

void ImageViewer::wheelEvent(QWheelEvent* e) {
    if(e->delta()!=0 && !m_fitInView) {
        qreal scaleFactor = e->delta()>0? m_zoomFactor : 1.0/m_zoomFactor;
        scale(scaleFactor,scaleFactor);
    }
    e->accept();
}
