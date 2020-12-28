#include "rvpickzmq.h"

/**
 * @file                rvpickzmq.cpp
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 */

RvPickzmq::RvPickzmq()
{
}

RvPickzmq::~RvPickzmq()
{
}

void RvPickzmq::setup(QString ip, int port)
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

void RvPickzmq::readZMQData()
{
    qsn->setEnabled(false);

    int events = 0;
    std::size_t eventsSize = sizeof(events);
    zmq_getsockopt(subscriber,ZMQ_EVENTS, &events, &eventsSize);
    if(events & ZMQ_POLLIN)
    {
        char string[sizeof(PICK_INFO)];
        PICK_INFO *pick;
        while(1)
        {        
            zmq_recv(subscriber, string, sizeof(PICK_INFO), 0);
            pick = (PICK_INFO *)string;

            //qWarning("--- %s", pick->sta);
            emit(sendPICKtoMain(pick));
            QThread::msleep(50);
            QCoreApplication::processEvents();
        }
    }

    qsn->setEnabled(true);
}
