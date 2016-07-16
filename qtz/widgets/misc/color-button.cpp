#include "color-button.hpp"
#include <QColorDialog>

ColorButton::ColorButton(QWidget *parent):
    QToolButton(parent)
{
    this->setStyleSheet("QToolButton{border: 1px solid black;}");
    connect(this,SIGNAL(clicked(bool)),this,SLOT(chooseColor()));
}

QColor ColorButton::currentColor() const
{
    return m_currentColor;
}

void ColorButton::setCurrentColor(const QColor &color)
{
    if(color.isValid()){
        setColor(color);
    }
}

void ColorButton::chooseColor()
{
    QColorDialog dlg(this);
    if(m_currentColor.isValid()){
        dlg.setCurrentColor(m_currentColor);
    }
    if(dlg.exec() == QColorDialog::Accepted){
        setColor(dlg.currentColor());
    }
}

void ColorButton::setColor(QColor color)
{
    if(color.isValid()){
        m_currentColor = color;
        QString style = QString("QToolButton{border: 1px solid black;"
                                "background-color: rgb(%1,%2,%3)"
                                ";}")
                .arg(color.red())
                .arg(color.green())
                .arg(color.blue())
                ;
        this->setStyleSheet(style);
    }
}
