#ifndef QTZ_WIDGETS_REMOVECOMMAND_HPP
#define QTZ_WIDGETS_REMOVECOMMAND_HPP

#include "hex-command.hpp"
#include "../../../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT RemoveCommand: public HexCommand {
public:
    RemoveCommand(GapBuffer* gapbuffer,
                  integer_t offset,
                  integer_t length,
                  QUndoCommand* parent = 0);
    virtual void undo();
    virtual void redo();
};

#endif // QTZ_WIDGETS_REMOVECOMMAND_HPP
