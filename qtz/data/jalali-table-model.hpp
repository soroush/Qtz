#ifndef QTZ_JALALISQLTABLEMODEL_HPP
#define QTZ_JALALISQLTABLEMODEL_HPP

#include <QSqlTableModel>
#include <QLocale>
#include "data.hpp"

#ifndef Q_NULLPTR
#if __cplusplus >= 201103L
#define Q_NULLPTR nullptr
#else
#define Q_NULLPTR NULL
#endif
#endif

class QTZ_DATA_SHARED_EXPORT JalaliSqlTableModel : public QSqlTableModel {
public:
    JalaliSqlTableModel(QObject* parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    ~JalaliSqlTableModel()=default;
    QVariant data(const QModelIndex& idx, int role) const;
    void setFormat(size_t index, const QString& format);
    QString format(size_t index);
private:
    QMap<size_t,QString> m_formats;
    QLocale m_locale;
};

#endif // QTZ_JALALISQLTABLEMODEL_HPP
