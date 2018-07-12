#ifndef ASYNC_MODEL_REGISTER_HPP
#define ASYNC_MODEL_REGISTER_HPP

#include <QObject>
#include <QMap>

class AsyncSqlTableModel;

class AsyncModelRegister : public QObject {
        Q_OBJECT
    public:
        enum MarkMethod {MarkAfterSelected, MarkAfterSubmitted};
        explicit AsyncModelRegister(QObject *parent = 0, MarkMethod method = MarkAfterSelected);

        void addModel(AsyncSqlTableModel *model);
        bool removeModel(AsyncSqlTableModel *model);
        bool removeAllModels();
        bool isDone() const;

        QList<AsyncSqlTableModel *> models() const;
    signals:
        void allMarked(bool);
    public slots:
        void mark();
        void clear();
    private:
        QMap<AsyncSqlTableModel *, bool> modelMap;
        MarkMethod method;
        int totalMarked;
};

#endif // ASYNC_MODEL_REGISTER_HPP
