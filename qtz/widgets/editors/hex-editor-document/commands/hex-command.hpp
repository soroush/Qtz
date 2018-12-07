#ifndef QTZ_WIDGETS_HEXCOMMAND_HPP
#define QTZ_WIDGETS_HEXCOMMAND_HPP

#include <QUndoCommand>
#include "../gap-buffer.hpp"
#include "../../../qtz-widgets.hpp"

class QTZ_WIDGETS_SHARED_EXPORT HexCommand: public QUndoCommand {
public:
    HexCommand(GapBuffer* gapbuffer, QUndoCommand* parent = 0);

protected:
    GapBuffer* _gapbuffer;
    integer_t _offset, _length;
    QByteArray _data;
};

#endif // QTZ_WIDGETS_HEXCOMMAND_HPP
