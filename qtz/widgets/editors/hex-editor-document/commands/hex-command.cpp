#include "hex-command.hpp"

HexCommand::HexCommand(GapBuffer* gapbuffer, QUndoCommand* parent):
    QUndoCommand(parent), _gapbuffer(gapbuffer), _offset(0),
    _length(0) {

}
