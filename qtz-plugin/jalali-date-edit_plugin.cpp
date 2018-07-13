#include <qtz/widgets/editors/jalali-date-edit.hpp>
#include "jalali-date-edit_plugin.hpp"

#include <QtCore/QtPlugin>

JalaliDateEditPlugin::JalaliDateEditPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.jalaliDateEditPlugin")
}

void JalaliDateEditPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool JalaliDateEditPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* JalaliDateEditPlugin::createWidget(QWidget* parent) {
    return new JalaliDateEdit(parent);
}

QString JalaliDateEditPlugin::name() const {
    return QLatin1String("JalaliDateEdit");
}

QString JalaliDateEditPlugin::group() const {
    return QLatin1String("Qtz Editors");
}

QIcon JalaliDateEditPlugin::icon() const {
    return QIcon::fromTheme("resource-calendar-insert",
                            QIcon{":/qtz_plugin/images/calendar.png"});
}

QString JalaliDateEditPlugin::toolTip() const {
    return QLatin1String("Jalali Date Edit Widget");
}

QString JalaliDateEditPlugin::whatsThis() const {
    return QLatin1String("This widget selects a day in jalali calendar");
}

bool JalaliDateEditPlugin::isContainer() const {
    return false;
}

QString JalaliDateEditPlugin::domXml() const {
    return QLatin1String("<widget class=\"JalaliDateEdit\" name=\"jalaliDateEdit\">\n</widget>\n");
}

QString JalaliDateEditPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/editors/jalali-date-edit.hpp");
}
