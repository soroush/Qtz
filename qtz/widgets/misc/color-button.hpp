#ifndef QTZ_COLOR_BUTTON_HPP
#define QTZ_COLOR_BUTTON_HPP

#include "../qtz-widgets.h"
#include <QToolButton>

class QTZ_WIDGETS_SHARED_EXPORT ColorButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor
               NOTIFY currentColorChanged)
public:
    ColorButton(QWidget* parent=nullptr);

    QColor currentColor() const;
public slots:
    void setCurrentColor(const QColor &color);

signals:
    void colorSelected(const QColor &color);
    void currentColorChanged(const QColor &color);
private slots:
    void chooseColor();
    void setColor(QColor);
private:
    QColor m_currentColor;
};

#endif // QTZ_COLOR_BUTTON_HPP
