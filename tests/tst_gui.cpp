#include "tst_gui.h"
#include <QTest>
#include <qtz/widgets/data/dialog-database-config.h>

#define LONG_DELAY 50
#define SHORT_DELAY 50

GuiTest::GuiTest(QObject *parent) :
    TestUnit(parent),
    db(static_cast<DatabaseTest *>(parent))
{
}

void GuiTest::initTestCase()
{
}

void GuiTest::databaseConfigDialog()
{
    DialogDatabaseConfig dialog;
    dialog.show();
    QTest::keyClicks(&dialog,"v",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(&dialog,"h",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(dialog.focusWidget(),this->db->getMysqlHost(),Qt::NoModifier,
                     LONG_DELAY);
    QTest::keyClicks(&dialog,"p",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(dialog.focusWidget(),this->db->getMysqlPort(),Qt::NoModifier,
                     LONG_DELAY);
    QTest::keyClicks(&dialog,"d",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(dialog.focusWidget(),this->db->getMysqlDB(),Qt::NoModifier,
                     LONG_DELAY);
    QTest::keyClicks(&dialog,"u",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(dialog.focusWidget(),this->db->getMysqlUserName(),
                     Qt::NoModifier,LONG_DELAY);
    QTest::keyClicks(&dialog,"w",Qt::AltModifier,SHORT_DELAY);
    QTest::keyClicks(dialog.focusWidget(),this->db->getMysqlPassword(),
                     Qt::NoModifier,LONG_DELAY);
    QTest::keyClicks(&dialog,"t",Qt::AltModifier,SHORT_DELAY);
    QTest::qWait(35000);
    QTest::mouseClick(dialog.focusWidget(),Qt::LeftButton,0,QPoint(),LONG_DELAY);
    QTest::keyClick(&dialog,Qt::Key_Enter,Qt::NoModifier,SHORT_DELAY);
}

void GuiTest::cleanupTestCase()
{
}
