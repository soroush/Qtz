#ifndef QTZ_SEXAGESIMALSPINBOX_HPP
#define QTZ_SEXAGESIMALSPINBOX_HPP

#include <QAbstractSpinBox>
#include <QWheelEvent>
#include <qtz/core/qtz-core.hpp>
#include "../qtz-widgets.hpp"

// TODO: Add Q_PROPERTY to widget
// TODO: Add Formatting options to this widget

class QTZ_WIDGETS_SHARED_EXPORT SexagesimalSpinBox : public QAbstractSpinBox {
    Q_OBJECT
public:
    explicit SexagesimalSpinBox(QWidget* parent = 0);
    enum class Type {
        Longitude,
        Latitude,
        Azimuth,
        Zenith,
        GenericAngle
    };
    enum class Unit {
        Degree,
        Radian
    };
    enum class LongitudeDirection {
        East,
        West
    };

signals:
    void valueChanged(double);

public slots:
    void setValue(const double&);
    void setValue(const int&, const int&, const double&);
    void updateText();

protected:
    void keyPressEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void wheelEvent(QWheelEvent* event);
    StepEnabled stepEnabled() const;

private:
    double m_value;
    int m_degree;
    int m_minute;
    double m_second;
    Type m_type;
    Unit m_unit;

    void addDegree(const int& d);
    void addMinute(const int& m);
    void addSecond(const int& s);
    void s2d();
    void d2s();
    void stepBy(int steps);

private slots:
    void setText(const QString& s);

};

#endif // SEXAGECIMALSPINBOX_H
