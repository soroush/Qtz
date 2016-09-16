#include "asynchronous-main-window.hpp"
#include <QStatusBar>

AsynchronousMainWindow::AsynchronousMainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_progressBar(new QProgressBar),
    m_progressDialog(new QProgressDialog(this)) {
    m_progressDialog->close();
    m_progressBar->setMaximum(0);
    m_progressBar->setMinimum(0);
    m_progressDialog->setMaximum(0);
    m_progressDialog->setMinimum(0);
    m_progressDialog->setWindowTitle(tr("Performing operatuion..."));
    m_progressDialog->setLabelText(tr("Performing asynchronous operation...\nPlease stand by."));
    m_progressDialog->setCancelButton(Q_NULLPTR);
    m_progressDialog->setWindowModality(Qt::WindowModal);
    // Remove close button on window
    m_progressDialog->setWindowFlags(m_progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
    // Create async connections
    connect(&m_dataWatcher,SIGNAL(started()),this,SLOT(disableUI()));
    connect(&m_dataWatcher,SIGNAL(finished()),this,SLOT(enableUI()));
}

AsynchronousMainWindow::~AsynchronousMainWindow() {
}

void AsynchronousMainWindow::setMode(AsynchronousMainWindow::Mode m) {
    m_mode = m;
}

AsynchronousMainWindow::Mode AsynchronousMainWindow::mode() const {
    return m_mode;
}

void AsynchronousMainWindow::enableUI() {
    this->setCursor(m_lastCursor);
    this->setEnabled(true);
    switch (m_mode) {
        case Mode::StatusBar:
            if(statusBar()!=Q_NULLPTR) {
                statusBar()->removeWidget(m_progressBar);
            }
            break;
        case Mode::Dialog:
            m_progressDialog->hide();
            break;
        default:
            break;
    }
}

void AsynchronousMainWindow::disableUI() {
    m_lastCursor = this->cursor();
    QCursor waitingCursor{Qt::WaitCursor};
    this->setCursor(waitingCursor);
    this->setDisabled(true);
    switch (m_mode) {
        case Mode::StatusBar:
            if(statusBar()!=Q_NULLPTR) {
                if((m_progressBar)==Q_NULLPTR) {
                    m_progressBar = new QProgressBar;
                    m_progressBar->setMaximum(0);
                    m_progressBar->setMinimum(0);
                }
                statusBar()->addWidget(m_progressBar);
                m_progressBar->show();
            }
        break;
        case Mode::Dialog:
            m_progressDialog->show();
            break;
        default:
            break;
    }
}
