#include "rvcoeffzmq.h"

/**
 * @file                rvcoeffzmq.cpp
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 */

RvCoeffzmq::RvCoeffzmq()
{
}

RvCoeffzmq::~RvCoeffzmq()
{
}

void RvCoeffzmq::setup(QString ip, int port)
{
    context = zmq_ctx_new ();
    subscriber = zmq_socket (context, ZMQ_SUB);

    fd = 0;
    fd_size = sizeof(fd);

    QString connectS = "tcp://" + ip + ":" + QString::number(port);
    QByteArray barray = connectS.toLocal8Bit();
    int rc = zmq_connect (subscriber, barray.data());

    char *filter = "";
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
    rc = zmq_getsockopt(subscriber, ZMQ_FD, &fd, &fd_size);

    qsn = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(qsn, SIGNAL(activated(int)), this, SLOT(readZMQData()), Qt::DirectConnection);
}

void RvCoeffzmq::readZMQData()
{
    qsn->setEnabled(false);

    int events = 0;
    std::size_t eventsSize = sizeof(events);
    zmq_getsockopt(subscriber,ZMQ_EVENTS, &events, &eventsSize);
    if(events & ZMQ_POLLIN)
    {
        char string[sizeof(COEFF)];
        COEFF *coeff;
        while(1)
        {        
            zmq_recv(subscriber, string, sizeof(COEFF), 0);
            coeff = (COEFF *)string;
            emit(sendCOEFFtoMain(coeff));
            QThread::msleep(100);
            QCoreApplication::processEvents();
        }
    }

    qsn->setEnabled(true);
}
