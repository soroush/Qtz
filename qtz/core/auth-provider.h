#ifndef AUTHPROVIDER_H
#define AUTHPROVIDER_H

#include <QObject>
#include <QMap>
#include <qtz/global.h>

class QTZ_SHARED_EXPORT AuthProvider : public QObject {
    Q_OBJECT
private:
    AuthProvider(QObject *parent = NULL):
        QObject(parent) {
    }
    AuthProvider(const AuthProvider &other):
        QObject(other.parent()) {
    }

    static AuthProvider *m_instance;
    static bool set;

    QString passwordFileName;
    QString authTableName;
    QString authIDFiled;
    QString authPassField;
    QMap<QString, QString> values;
    void setValue(const QString &name, const QString &value);

public:
    QString decryptPassword(const QString &);
    QString encryptPassword(const QString &);
    bool checkLogin(const QString &username, const QString &password);
    void initialize(const QString &_authTableName = "users",
                    const QString &_authIDFiled = "username",
                    const QString &_authPassField = "password");
    static AuthProvider *instance();
    QString value(const QString &name);

protected:
    virtual void getData();

signals:

public slots:

};

#endif // AUTHPROVIDER_H
