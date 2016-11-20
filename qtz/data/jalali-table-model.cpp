#include "jalali-table-model.hpp"
#include <qtz/core/jalali-date.hpp>

JalaliSqlTableModel::JalaliSqlTableModel(QObject* parent, QSqlDatabase db):
    QSqlTableModel(parent,db) {
}

QVariant JalaliSqlTableModel::data(const QModelIndex& idx, int role) const {
    QVariant v=QSqlTableModel::data(idx,role);
    if(role==Qt::DisplayRole) {
        switch(v.type()) {
            case QVariant::DateTime: {
                JalaliDate j(v.toDate());
                if(m_formats.find(idx.column())!=m_formats.end()) {
                    return QString("%1 %2")
                           .arg(j.toString(m_formats[idx.column()]))
                           .arg(m_locale.toString(v.toTime(),"HH:mm:ss"));
                } else {
                    return j.toString() + ' ' +
                           m_locale.toString(v.toTime(),"HH:mm:ss");
                }
            }
            break;
            case QVariant::Date: {
                JalaliDate j(v.toDate());
                if(m_formats.find(idx.column())!=m_formats.end()) {
                    return j.toString(m_formats[idx.column()]);
                } else {
                    return j.toString();
                }
            }
            break;
            default:
                return v;
                break;
        }
    }
    return v;
}

void JalaliSqlTableModel::setFormat(size_t index, const QString& format) {
    m_formats[index]=format;
}

QString JalaliSqlTableModel::format(size_t index) {
    if(m_formats.find(index) != m_formats.end()) {
        return m_formats[index];
    }
    return QString{};
}
