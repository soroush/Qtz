#ifndef TST_GUI_H
#define TST_GUI_H

#include "testunit.h"
#include "tst_database.h"

class GuiTest : public TestUnit
{
    Q_OBJECT
public:
    explicit GuiTest(QObject *parent = nullptr);
    QStringList parseArguments(int argc, char *argv[]){}
    void parseArguments(QStringList& arguments){}
    
private slots:
    void initTestCase();
    void databaseConfigDialog();
    void cleanupTestCase();

private:
    DatabaseTest *db;
};

#endif // TST_GUI_H
