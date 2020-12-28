/**
 */

#ifndef CALLLOCATION_H
#define CALLLOCATION_H

#include <QObject>
#include <QDir>
#include <QDebug>

#include "common.h"

class CallLocation : public QObject
{
    Q_OBJECT

public:
    explicit CallLocation();
    ~CallLocation();

    void setup(QString pickDir, int locStanRef, double firstPick, QString nkInfoFile, double gridlatB, double gridlatE, double gridlonB, double gridlonE);

public slots:
    void getLoc();

private:
    QVector<STATION_FOR_LOC> staVector;
    QVector<double> grid_lat;
    QVector<double> grid_lon;
    QVector<GRID> grid_vector;
    QVector<RESULT_FOR_LOC> result_vector;
    QVector<double> obs;
    QVector< QVector<double> > pred;
    QVector<double> result;

    double min, max; int location_key; double result_lat, result_lon;

    QString PickDir, NKInfoFile;
    int LocStanRef;
    double FirstPick, GridLatB, GridLatE, GridLonB, GridLonE;

private slots:

signals:
    void finished();
};

#endif // CALLLOCATION_H
