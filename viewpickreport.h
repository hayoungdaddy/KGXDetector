/**
 * @file                viewpickreport.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               ViewPickReport class
 */

#ifndef VIEWPICKREPORT_H
#define VIEWPICKREPORT_H

#include <QDialog>

#include "qcustomplot.h"
#include "common.h"

namespace Ui {
class ViewPickReport;
}

class ViewPickReport : public QDialog
{
    Q_OBJECT
    
public:
    explicit ViewPickReport(QString path = 0, QWidget *parent = 0);
    ~ViewPickReport();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

private:
    Ui::ViewPickReport *ui;
    QCPItemTracer *itemDemoPhaseTracer;

    int nRefData, pOffset, sps;
    QString coeffHTime, coeffETime, sta, chan, threshold, pTime, maxAmp, per, maxAmpTime, mb, coeff[MAX_NUM_REFDATA];
    double delta, maxAmpT, minAmpT, nSToMax, thresholdD;

    void setWave(QString, QCustomPlot *customPlot);

signals:
    void finished();
};

#endif // VIEWPICKREPORT_H
