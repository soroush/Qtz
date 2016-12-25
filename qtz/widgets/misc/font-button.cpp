#include "font-button.hpp"
#include <QFontDialog>
#include <QLocale>

FontButton::FontButton(QWidget* parent):
    QToolButton(parent) {
    connect(this,SIGNAL(clicked(bool)),this,SLOT(chooseFont()));
}

void FontButton::setFont(const QFont& font) {
    QToolButton::setFont(font);
    if(font!=m_currentFont) {
        m_currentFont = font;
        QString fontDisplay = font.family()+", "+
                              QLocale().toString(font.pointSize())+tr("pt");
        if(font.bold()) {
            fontDisplay+= tr(", Bold");
        }
        if(font.italic()) {
            fontDisplay+= tr(", Italic");
        }
        this->setText(fontDisplay);
        emit fontSelected(m_currentFont);
    }
}

void FontButton::chooseFont() {
    QFontDialog dlg(this);
    if(dlg.exec()==QFontDialog::Accepted) {
        if(dlg.selectedFont()!=m_currentFont) {
            setFont(dlg.selectedFont());
        }
    }
}
