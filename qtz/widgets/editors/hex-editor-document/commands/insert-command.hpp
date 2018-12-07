#ifndef QTZ_WIDGETS_INSERTCOMMAND_HPP
#define QTZ_WIDGETS_INSERTCOMMAND_HPP

#include "hex-command.hpp"
#include "../../../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT InsertCommand: public HexCommand {
public:
    InsertCommand(GapBuffer* gapbuffer, integer_t offset,
                  const QByteArray& data, QUndoCommand* parent = 0);
    virtual void undo();
    virtual void redo();
};

#endif // QTZ_WIDGETS_INSERTCOMMAND_HPP
