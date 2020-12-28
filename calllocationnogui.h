/**
 */

#ifndef CALLLOCATIONNOGUI_H
#define CALLLOCATIONNOGUI_H

#include <QObject>
#include <QDir>
#include <QDebug>

#include "common.h"

class CallLocationNoGUI : public QObject
{
    Q_OBJECT

public:
    explicit CallLocationNoGUI();
    ~CallLocationNoGUI();

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
    double GridLatB, GridLatE, GridLonB, GridLonE;
    QVector<QString> StaList;
    QVector<double> PArrivalTime;

    QString refOTS, refLatS, refLonS;
    QStringList refStaList, refATList;

    void makeGrid(double gridlatB, double gridlatE, double gridlonB, double gridlonE, double spacing);
    void ready(QString pickDir, int locStanRef, QString nkInfoFile);

    void setup(QVector<QString> staList, QVector<double> pArrivalTime);

public slots:
    void getLoc();

private:

private slots:

signals:
    void finished(double, double, double, double);
    void killme();
};

#endif // CALLLOCATIONNOGUI_H
