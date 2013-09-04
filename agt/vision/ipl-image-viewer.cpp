#include "ipl-image-viewer.h"
#include <QPainter>
#include <opencv2/highgui/highgui.hpp>

IplImageViewer::IplImageViewer(QWidget *parent) :
    QWidget(parent)
{
}

void IplImageViewer::setImage(IplImage* _image)
{
    cvReleaseImage(&m_image);
    m_image = _image;
    update();
}

void IplImageViewer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(m_image!=NULL)
    {
        m_qimage = new QImage((uchar*)m_image->imageData,
                            m_image->width,
                            m_image->height,
                            m_image->widthStep,
                            QImage::Format_RGB888);
        painter.drawImage(QPointF(0,0),*m_qimage);
        delete m_qimage;
        setMinimumSize(m_image->width,m_image->height);
    }
    QWidget::paintEvent(e);
}
