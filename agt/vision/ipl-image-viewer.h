#ifndef IPLIMAGEVIEWER_H
#define IPLIMAGEVIEWER_H

#include <QWidget>
#include <opencv2/core/core.hpp>

class IplImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit IplImageViewer(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
private:
    IplImage* m_image;
    QImage *m_qimage;
signals:
    void imageChanged();
public slots:
    void setImage(IplImage*);
    
public slots:
    
};

#endif // IPLIMAGEVIEWER_H
