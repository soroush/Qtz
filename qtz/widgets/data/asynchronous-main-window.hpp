#ifndef QTZ_ASYNCHRONOUSMAINWINDOW_HPP
#define QTZ_ASYNCHRONOUSMAINWINDOW_HPP

#include "../qtz-widgets.hpp"
#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressBar>
#include <QProgressDialog>

class QTZ_WIDGETS_SHARED_EXPORT AsynchronousMainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum class Mode {
        StatusBar,
        Dialog
    };
    explicit AsynchronousMainWindow(QWidget* parent = 0);
    virtual ~AsynchronousMainWindow();
    void setMode(Mode);
    Mode mode() const;
protected slots:
    // locker slots
    void enableUI();
    void disableUI();
protected:

    Mode m_mode;
    QFuture<void> m_dataFuture;
    QFutureWatcher<void> m_dataWatcher;
    QProgressBar* m_progressBar;
    QProgressDialog* m_progressDialog;
    QCursor m_lastCursor;
};

#endif // QTZ_ASYNCHRONOUSMAINWINDOW_HPP
