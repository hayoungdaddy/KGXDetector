/**
 * @file                rvcoeffzmq.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               RvCoeffzmq class
 */

#ifndef RVCOEFFZMQ_H
#define RVCOEFFZMQ_H

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

#include "common.h"
#include "zmq.h"
#include "zmq_utils.h"

class RvCoeffzmq : public QObject
{
    Q_OBJECT
public:
    RvCoeffzmq();
    ~RvCoeffzmq();

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
    void sendCOEFFtoMain(COEFF *);
};

#endif // RVCOEFFZMQ_H
