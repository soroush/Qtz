#ifndef TABLENODE_H
#define TABLENODE_H

#include <QList>

struct TableNode{
public:
    TableNode(const QString& _name, const int& _freedom):
        name(_name), degreeOfFreedom(_freedom)
    {}

    QList<TableNode*> referencedTables;
    QString name;
    int degreeOfFreedom;
};

#endif // TABLENODE_H
