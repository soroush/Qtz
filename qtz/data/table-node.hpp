#ifndef QTZ_TABLENODE_HPP
#define QTZ_TABLENODE_HPP

#include <QList>
#include "data.hpp"

struct QTZ_DATA_SHARED_EXPORT TableNode {
public:
    TableNode(const QString& _name, const int& _freedom):
        name(_name), degreeOfFreedom(_freedom) {
    }

    QList<TableNode*> referencedTables;
    QString name;
    int degreeOfFreedom;
};

#endif // QTZ_TABLENODE_HPP
