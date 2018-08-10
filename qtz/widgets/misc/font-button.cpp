#include "font-button.hpp"
#include <QFontDialog>
#include <QLocale>

FontButton::FontButton(QWidget* parent):
    QToolButton(parent) {
    connect(this, &QToolButton::clicked,
            this, &FontButton::selectFont);
}

void FontButton::setFont(const QFont& newFont) {
    QToolButton::setFont(newFont);
    if(newFont != font()) {
        QString fontDisplay = newFont.family() + ", " +
                              QLocale().toString(newFont.pointSize()) + tr("pt");
        if(newFont.bold()) {
            fontDisplay += tr(", Bold");
        }
        if(newFont.italic()) {
            fontDisplay += tr(", Italic");
        }
        this->setText(fontDisplay);
        emit fontChanged(font());
    }
}

void FontButton::selectFont() {
    QFontDialog dlg(this);
    if(dlg.exec() == QFontDialog::Accepted) {
        if(dlg.selectedFont() != font()) {
            setFont(dlg.selectedFont());
        }
    }
}
