/**
 * @file                rvpickzmq.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               RvPickzmq class
 */

#ifndef RVPICKZMQ_H
#define RVPICKZMQ_H

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

#include "common.h"
#include "zmq.h"
#include "zmq_utils.h"

class RvPickzmq : public QObject
{
    Q_OBJECT
public:
    RvPickzmq();
    ~RvPickzmq();

    QSocketNotifier *qsn;
    void *context;
    void *subscriber;

    unsigned int fd;
    size_t fd_size;

    void setup(QString, int);

private:

private slots:
    void readZMQData();

signals:
    void sendPICKtoMain(PICK_INFO *);
};

#endif // RVPICKZMQ_H
