#ifndef QTZ_IMAGEVIEWER_HPP
#define QTZ_IMAGEVIEWER_HPP

#include <QGraphicsView>
#include <QPointF>
// Don't know why?
#include <opencv2/core/version.hpp>
#if   (CV_VERSION_EPOCH==2)
#include <opencv2/highgui/highgui.hpp>
#elif (CV_VERSION_EPOCH==3)
#include <opencv2/highgui.hpp>
#endif


#include "../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT ImageViewer : public QGraphicsView {
        Q_OBJECT
    public:
        explicit ImageViewer(QWidget* parent = 0);
        QPixmap image() const;
        bool isFitInView() const;
        qreal zoomFactor() const;

    signals:

    public slots:
        void setImage(const QString& filePath);
        void setImage(const QPixmap& image);
        void setImage(const cv::Mat& image);
        void setFitInView(bool);
        void setZoomFactor(qreal factor);
    protected:
        void resizeEvent(QResizeEvent*);
        void wheelEvent(QWheelEvent*);
    private:
        bool m_fitInView;
        bool m_isMoving;
        qreal m_zoomFactor;
        QPointF m_lastPosition;
        QCursor m_defaultCursor;
        QPixmap m_pixmap;
};

#endif // QTZ_IMAGEVIEWER_HPP
