// The MIT License (MIT)
//
// Copyright (c) Itay Grudev 2015 - 2016
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <cstdlib>

#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QSemaphore>
#include <QtCore/QSharedMemory>
#include <QtCore/QStandardPaths>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDataStream>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#ifdef Q_OS_UNIX
#include <signal.h>
#include <unistd.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <lmcons.h>
#endif

#include "qtz-single-core-application.hpp"
#include "qtz-single-core-application_p.hpp"


QtzSingleCoreApplicationPrivate::QtzSingleCoreApplicationPrivate(QtzSingleCoreApplication *q_ptr) : q_ptr(q_ptr)
{
    server = nullptr;
    socket = nullptr;
}

QtzSingleCoreApplicationPrivate::~QtzSingleCoreApplicationPrivate()
{
    if(socket != nullptr) {
        socket->close();
        delete socket;
    }

    memory->lock();
    InstancesInfo *inst = static_cast<InstancesInfo *>(memory->data());
    if(server != nullptr) {
        server->close();
        delete server;
        inst->primary = false;
        inst->primaryPid = -1;
    }
    memory->unlock();

    delete memory;
}

void QtzSingleCoreApplicationPrivate::genBlockServerName(int timeout)
{
    QCryptographicHash appData(QCryptographicHash::Sha256);
    appData.addData("QtzSingleApplication", 17);
    appData.addData(QCoreApplication::applicationName().toUtf8());
    appData.addData(QCoreApplication::organizationName().toUtf8());
    appData.addData(QCoreApplication::organizationDomain().toUtf8());

    if(!(options & QtzSingleCoreApplication::Mode::ExcludeAppVersion)) {
        appData.addData(QCoreApplication::applicationVersion().toUtf8());
    }

    if(!(options & QtzSingleCoreApplication::Mode::ExcludeAppPath)) {
#ifdef Q_OS_WIN
        appData.addData(QCoreApplication::applicationFilePath().toLower().toUtf8());
#else
        appData.addData(QCoreApplication::applicationFilePath().toUtf8());
#endif
    }

    // User level block requires a user specific data in the hash
    if(options & QtzSingleCoreApplication::Mode::User) {
#ifdef Q_OS_WIN
        Q_UNUSED(timeout);
        wchar_t username [ UNLEN + 1 ];
        // Specifies size of the buffer on input
        DWORD usernameLength = UNLEN + 1;
        if(GetUserNameW(username, &usernameLength)) {
            appData.addData(QString::fromWCharArray(username).toUtf8());
        } else {
            appData.addData(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join("").toUtf8());
        }
#endif
#ifdef Q_OS_UNIX
        QProcess process;
        process.start("whoami");
        if(process.waitForFinished(timeout) &&
                process.exitCode() == QProcess::NormalExit) {
            appData.addData(process.readLine());
        } else {
            appData.addData(
                QDir(
                    QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first()
                ).absolutePath().toUtf8()
            );
        }
#endif
    }

    // Replace the backslash in RFC 2045 Base64 [a-zA-Z0-9+/=] to comply with
    // server naming requirements.
    blockServerName = appData.result().toBase64().replace("/", "_");
}

void QtzSingleCoreApplicationPrivate::startPrimary(bool resetMemory)
{
    Q_Q(QtzSingleCoreApplication);

#ifdef Q_OS_UNIX
    // Handle any further termination signals to ensure the
    // QSharedMemory block is deleted even if the process crashes
    crashHandler();
#endif
    // Successful creation means that no main process exists
    // So we start a QLocalServer to listen for connections
    QLocalServer::removeServer(blockServerName);
    server = new QLocalServer();

    // Restrict access to the socket according to the
    // QtzSingleApplication::Mode::User flag on User level or no restrictions
    if(options & QtzSingleCoreApplication::Mode::User) {
        server->setSocketOptions(QLocalServer::UserAccessOption);
    } else {
        server->setSocketOptions(QLocalServer::WorldAccessOption);
    }

    server->listen(blockServerName);
    QObject::connect(
        server,
        &QLocalServer::newConnection,
        this,
        &QtzSingleCoreApplicationPrivate::slotConnectionEstablished
    );

    // Reset the number of connections
    memory->lock();
    InstancesInfo *inst = static_cast<InstancesInfo *>(memory->data());

    if(resetMemory) {
        inst->secondary = 0;
    }

    inst->primary = true;
    inst->primaryPid = q->applicationPid();

    memory->unlock();

    instanceNumber = 0;
}

void QtzSingleCoreApplicationPrivate::startSecondary()
{
#ifdef Q_OS_UNIX
    // Handle any further termination signals to ensure the
    // QSharedMemory block is deleted even if the process crashes
    crashHandler();
#endif
}

void QtzSingleCoreApplicationPrivate::connectToPrimary(int msecs, ConnectionType connectionType)
{
    // Connect to the Local Server of the Primary Instance if not already
    // connected.
    if(socket == nullptr) {
        socket = new QLocalSocket();
    }

    // If already connected - we are done;
    if(socket->state() == QLocalSocket::ConnectedState)
        return;

    // If not connect
    if(socket->state() == QLocalSocket::UnconnectedState ||
            socket->state() == QLocalSocket::ClosingState) {
        socket->connectToServer(blockServerName);
    }

    // Wait for being connected
    if(socket->state() == QLocalSocket::ConnectingState) {
        socket->waitForConnected(msecs);
    }

    // Initialisation message according to the QtzSingleApplication protocol
    if(socket->state() == QLocalSocket::ConnectedState) {
        // Notify the parent that a new instance had been started;
        QByteArray initMsg;
        QDataStream writeStream(&initMsg, QIODevice::WriteOnly);
        writeStream.setVersion(QDataStream::Qt_5_6);
        writeStream << blockServerName.toLatin1();
        writeStream << static_cast<quint8>(connectionType);
        writeStream << instanceNumber;
        quint16 checksum = qChecksum(initMsg.constData(), static_cast<quint32>(initMsg.length()));
        writeStream << checksum;

        socket->write(initMsg);
        socket->flush();
        socket->waitForBytesWritten(msecs);
    }
}

qint64 QtzSingleCoreApplicationPrivate::primaryPid()
{
    qint64 pid;

    memory->lock();
    InstancesInfo *inst = static_cast<InstancesInfo *>(memory->data());
    pid = inst->primaryPid;
    memory->unlock();

    return pid;
}

#ifdef Q_OS_UNIX
void QtzSingleCoreApplicationPrivate::crashHandler()
{
    // Handle any further termination signals to ensure the
    // QSharedMemory block is deleted even if the process crashes
    signal(SIGHUP,  QtzSingleCoreApplicationPrivate::terminate);   // 1
    signal(SIGINT,  QtzSingleCoreApplicationPrivate::terminate);   // 2
    signal(SIGQUIT, QtzSingleCoreApplicationPrivate::terminate);   // 3
    signal(SIGILL,  QtzSingleCoreApplicationPrivate::terminate);   // 4
    signal(SIGABRT, QtzSingleCoreApplicationPrivate::terminate);   // 6
    signal(SIGFPE,  QtzSingleCoreApplicationPrivate::terminate);   // 8
    signal(SIGBUS,  QtzSingleCoreApplicationPrivate::terminate);   // 10
    signal(SIGSEGV, QtzSingleCoreApplicationPrivate::terminate);   // 11
    signal(SIGSYS,  QtzSingleCoreApplicationPrivate::terminate);   // 12
    signal(SIGPIPE, QtzSingleCoreApplicationPrivate::terminate);   // 13
    signal(SIGALRM, QtzSingleCoreApplicationPrivate::terminate);   // 14
    signal(SIGTERM, QtzSingleCoreApplicationPrivate::terminate);   // 15
    signal(SIGXCPU, QtzSingleCoreApplicationPrivate::terminate);   // 24
    signal(SIGXFSZ, QtzSingleCoreApplicationPrivate::terminate);   // 25
}

void QtzSingleCoreApplicationPrivate::terminate(int signum)
{
    delete((QtzSingleCoreApplication *)QCoreApplication::instance())->d_ptr;
    ::exit(128 + signum);
}
#endif

/**
 * @brief Executed when a connection has been made to the LocalServer
 */
void QtzSingleCoreApplicationPrivate::slotConnectionEstablished()
{
    Q_Q(QtzSingleCoreApplication);

    QLocalSocket *nextConnSocket = server->nextPendingConnection();

    quint32 instanceId = 0;
    ConnectionType connectionType = InvalidConnection;
    if(nextConnSocket->waitForReadyRead(100)) {
        // read all data from message in same order/format as written
        QByteArray msgBytes = nextConnSocket->read(nextConnSocket->bytesAvailable() - static_cast<qint64>(sizeof(quint16)));
        QByteArray checksumBytes = nextConnSocket->read(sizeof(quint16));
        QDataStream readStream(msgBytes);
        readStream.setVersion(QDataStream::Qt_5_6);

        // server name
        QByteArray latin1Name;
        readStream >> latin1Name;
        // connectioon type
        quint8 connType = InvalidConnection;
        readStream >> connType;
        connectionType = static_cast<ConnectionType>(connType);
        // instance id
        readStream >> instanceId;
        // checksum
        quint16 msgChecksum = 0;
        QDataStream checksumStream(checksumBytes);
        checksumStream.setVersion(QDataStream::Qt_5_6);
        checksumStream >> msgChecksum;

        const quint16 actualChecksum = qChecksum(msgBytes.constData(), static_cast<quint32>(msgBytes.length()));

        if(readStream.status() != QDataStream::Ok || QLatin1String(latin1Name) != blockServerName || msgChecksum != actualChecksum) {
            connectionType = InvalidConnection;
        }
    }

    if(connectionType == InvalidConnection) {
        nextConnSocket->close();
        delete nextConnSocket;
        return;
    }

    QObject::connect(
        nextConnSocket,
        &QLocalSocket::aboutToClose,
        this,
    [nextConnSocket, instanceId, this]() {
        Q_EMIT this->slotClientConnectionClosed(nextConnSocket, instanceId);
    }
    );

    QObject::connect(
        nextConnSocket,
        &QLocalSocket::readyRead,
        this,
    [nextConnSocket, instanceId, this]() {
        Q_EMIT this->slotDataAvailable(nextConnSocket, instanceId);
    }
    );

    if(connectionType == NewInstance || (
                connectionType == SecondaryInstance &&
                options & QtzSingleCoreApplication::Mode::SecondaryNotification
            )
      ) {
        Q_EMIT q->instanceStarted();
    }

    if(nextConnSocket->bytesAvailable() > 0) {
        Q_EMIT this->slotDataAvailable(nextConnSocket, instanceId);
    }
}

void QtzSingleCoreApplicationPrivate::slotDataAvailable(QLocalSocket *dataSocket, quint32 instanceId)
{
    Q_Q(QtzSingleCoreApplication);
    Q_EMIT q->receivedMessage(instanceId, dataSocket->readAll());
}

void QtzSingleCoreApplicationPrivate::slotClientConnectionClosed(QLocalSocket *closedSocket, quint32 instanceId)
{
    if(closedSocket->bytesAvailable() > 0)
        Q_EMIT slotDataAvailable(closedSocket, instanceId);
    closedSocket->deleteLater();
}

/**
 * @brief Constructor. Checks and fires up LocalServer or closes the program
 * if another instance already exists
 * @param argc
 * @param argv
 * @param {bool} allowSecondaryInstances
 */
QtzSingleCoreApplication::QtzSingleCoreApplication(int &argc, char *argv[], bool allowSecondary, Options options, int timeout)
    : QCoreApplication(argc, argv), d_ptr(new QtzSingleCoreApplicationPrivate(this))
{
    Q_D(QtzSingleCoreApplication);

    // Store the current mode of the program
    d->options = options;

    // Generating an application ID used for identifying the shared memory
    // block and QLocalServer
    d->genBlockServerName(timeout);

    // Guarantee thread safe behaviour with a shared memory block. Also by
    // explicitly attaching it and then deleting it we make sure that the
    // memory is deleted even if the process had crashed on Unix.
#ifdef Q_OS_UNIX
    d->memory = new QSharedMemory(d->blockServerName);
    d->memory->attach();
    delete d->memory;
#endif
    d->memory = new QSharedMemory(d->blockServerName);

    // Create a shared memory block
    if(d->memory->create(sizeof(InstancesInfo))) {
        d->startPrimary(true);
        return;
    } else {
        // Attempt to attach to the memory segment
        if(d->memory->attach()) {
            d->memory->lock();
            InstancesInfo *inst = static_cast<InstancesInfo *>(d->memory->data());

            if(! inst->primary) {
                d->startPrimary(false);
                d->memory->unlock();
                return;
            }

            // Check if another instance can be started
            if(allowSecondary) {
                inst->secondary += 1;
                d->instanceNumber = inst->secondary;
                d->startSecondary();
                if(d->options & Mode::SecondaryNotification) {
                    d->connectToPrimary(timeout, QtzSingleCoreApplicationPrivate::SecondaryInstance);
                }
                d->memory->unlock();
                return;
            }

            d->memory->unlock();
        }
    }

    d->connectToPrimary(timeout, QtzSingleCoreApplicationPrivate::NewInstance);
    delete d;
    ::exit(EXIT_SUCCESS);
}

/**
 * @brief Destructor
 */
QtzSingleCoreApplication::~QtzSingleCoreApplication()
{
    Q_D(QtzSingleCoreApplication);
    delete d;
}

bool QtzSingleCoreApplication::isPrimary()
{
    Q_D(QtzSingleCoreApplication);
    return d->server != nullptr;
}

bool QtzSingleCoreApplication::isSecondary()
{
    Q_D(QtzSingleCoreApplication);
    return d->server == nullptr;
}

quint32 QtzSingleCoreApplication::instanceId()
{
    Q_D(QtzSingleCoreApplication);
    return d->instanceNumber;
}

qint64 QtzSingleCoreApplication::primaryPid()
{
    Q_D(QtzSingleCoreApplication);
    return d->primaryPid();
}

bool QtzSingleCoreApplication::sendMessage(QByteArray message, int timeout)
{
    Q_D(QtzSingleCoreApplication);

    // Nobody to connect to
    if(isPrimary()) {
        return false;
    }

    // Make sure the socket is connected
    d->connectToPrimary(timeout,  QtzSingleCoreApplicationPrivate::Reconnect);

    d->socket->write(message);
    bool dataWritten = d->socket->flush();
    d->socket->waitForBytesWritten(timeout);
    return dataWritten;
}
