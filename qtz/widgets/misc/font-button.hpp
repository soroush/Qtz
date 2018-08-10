#ifndef QTZ_FONT_BUTTON_HPP
#define QTZ_FONT_BUTTON_HPP

#include <QToolButton>
#include "../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT FontButton : public QToolButton {
    Q_OBJECT
public:
    FontButton(QWidget* parent = nullptr);
public slots:
    void setFont(const QFont& font);
    void selectFont();
signals:
    void fontChanged(QFont);
};

#endif // QTZ_FONT_BUTTON_HPP
