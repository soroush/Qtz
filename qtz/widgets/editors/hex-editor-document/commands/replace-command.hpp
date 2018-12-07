#ifndef QTZ_WIDGETS_REPLACECOMMAND_HPP
#define QTZ_WIDGETS_REPLACECOMMAND_HPP

#include "hex-command.hpp"
#include "../../../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT ReplaceCommand: public HexCommand {
public:
    ReplaceCommand(GapBuffer* gapbuffer, integer_t offset, const QByteArray& data, QUndoCommand* parent = 0);
    virtual void undo();
    virtual void redo();

private:
    QByteArray _olddata;
};

#endif // QTZ_WIDGETS_REPLACECOMMAND_HPP
