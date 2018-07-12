#ifndef ASYNC_DATABASE_EXCEPTION_HPP
#define ASYNC_DATABASE_EXCEPTION_HPP

#include <exception>
#include <QString>
#include <QSqlError>

class DatabaseException : private std::exception {
    public:
        enum ErrorCode {FailedToCommit = 10000, DuplicateEntry};
        DatabaseException(const QSqlError &e, const QString m_userMessage = "");
        ~DatabaseException() throw() = default;
        const char *what() const throw();
        QSqlError getError() const;
    private:
        int m_code;
        QString m_message;
        QString m_userMessage;
        QSqlError m_error;
};

#endif // ASYNC_DATABASE_EXCEPTION_HPP
