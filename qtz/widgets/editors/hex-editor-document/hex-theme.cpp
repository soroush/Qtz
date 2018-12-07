#include "hex-theme.hpp"
#include <QGuiApplication>
#include <QFontDatabase>
#include <QPalette>
#include <QWidget>

HexTheme::HexTheme(QObject* parent): QObject(parent) {
    QWidget* container = static_cast<QWidget*>(parent);

    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    container->setFont(f);
    container->setBackgroundRole(QPalette::Base);

    this->_selectedcursor = QColor(Qt::lightGray);
    this->_addressbackground = QColor(0xF0, 0xF0, 0xFE);
    this->_addressforeground = QColor(Qt::darkBlue);
    this->_alternateaddressforeground = QColor(Qt::red);
    this->_alternatelinecolor = QColor(0xF0, 0xF0, 0xFE);
    this->_linecolor = QColor(0xFF, 0xFF, 0xA0);
}

QColor HexTheme::baseColor() const {
    return this->_basecolor;
}

QColor HexTheme::selectedCursor() const {
    return this->_selectedcursor;
}

QColor HexTheme::addressBackground() const {
    return this->_addressbackground;
}

QColor HexTheme::addressForeground() const {
    return this->_addressforeground;
}

QColor HexTheme::alternateAddressForeground() const {
    return this->_alternateaddressforeground;
}

QColor HexTheme::alternateLineColor() const {
    return this->_alternatelinecolor;
}

QColor HexTheme::lineColor() const {
    return this->_linecolor;
}

void HexTheme::setBaseColor(const QColor& color) {
    if(this->_basecolor == color) {
        return;
    }

    this->_basecolor = color;
    emit themeChanged();
}

void HexTheme::setSelectedCursor(const QColor& color) {
    if(this->_selectedcursor == color) {
        return;
    }

    this->_selectedcursor = color;
    emit themeChanged();
}

void HexTheme::setAddressBackground(const QColor& color) {
    if(this->_addressbackground == color) {
        return;
    }

    this->_addressbackground = color;
    emit themeChanged();
}

void HexTheme::setAddressForeground(const QColor& color) {
    if(this->_addressforeground == color) {
        return;
    }

    this->_addressforeground = color;
    emit themeChanged();
}

void HexTheme::setAlternateAddressForeground(const QColor& color) {
    if(this->_alternateaddressforeground == color) {
        return;
    }

    this->_alternateaddressforeground = color;
    emit themeChanged();
}

void HexTheme::setAlternateLineColor(const QColor& color) {
    if(this->_alternatelinecolor == color) {
        return;
    }

    this->_alternatelinecolor = color;
    emit themeChanged();
}

void HexTheme::setLineColor(const QColor& color) {
    if(this->_linecolor == color) {
        return;
    }

    this->_linecolor = color;
    emit themeChanged();
}
