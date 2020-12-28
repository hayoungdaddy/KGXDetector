#ifndef LOCGRAPH_H
#define LOCGRAPH_H

#include <QDialog>

#include "qcustomplot.h"
#include "common.h"

namespace Ui {
class LocGraph;
}

class LocGraph : public QDialog
{
    Q_OBJECT

public:
    explicit LocGraph(QString rootDir = 0, QString firstPick = 0, QString nkInfoFile = 0, int locStanRef = 0, QWidget *parent = 0);
    ~LocGraph();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

    QString newName;

private:
    Ui::LocGraph *ui;

    void readLocFile(QString);
    void readNKFile(QString);
    void setGraph(QCustomPlot *customPlot);
    double thisLat, thisLon;
    int LocStanRef;

    QVector<double> x, y, z;

signals:
    void finished();
};

#endif // LOCGRAPH_H
