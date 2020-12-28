/**
 * @file                mainwindow.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               MainWindow class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QPainter>

#include "common.h"
#include "config.h"
#include "rvcoeffzmq.h"
#include "rvpickzmq.h"
#include "viewpickreport.h"
#include "eventlist.h"
#include "mbgraph.h"
#include "calllocation.h"
#include "calllocationnogui.h"
#include "locgraph.h"

#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString configFile = 0, QWidget *parent = 0);
    //explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;

    CONFIGURE con;
    EVENT event;

    /* for graph */
    int isPlay;
    int xSizeId, xSizeArray[4];
    int ySizeId;

    double ccLastTime;
    QDateTime systemTime;

    QLabel *staNameL[MAX_SCNL];
    QLabel *timeL[MAX_SCNL];
    QLabel *coeffL[MAX_SCNL];
    QLabel *mbL[MAX_SCNL];
    QPushButton *detailB[MAX_SCNL];
    QHBoxLayout *myLayout;
    QLabel *legend1, *legend2, *legend3;
    QLabel *avgLabel;
    QLabel *avgCoeffL, *avgMbL;
    QPushButton *viewmbB;
    QFrame *line;

    QCustomPlot *plot[MAX_SCNL];
    QCustomPlot *ccPlot;

    /* widget */
    Config *config;
    RvCoeffzmq *rvcoeffzmq;
    RvPickzmq *rvpickzmq;

    /* timer */
    QTimer *ccTimer;
    QTimer *sohTimer;
    QTimer *blinkTimer;
    QTimer *sysTimer;

    /* thread */
    QThread *rvCoeffThread;
    QThread *rvPickThread;
    QThread *getFinalLocThread;

    /* for cc */
    QMultiMap<double, COEFFMAP> mmap;
    QVector<double> maxCoeffV[MAX_SCNL];
    QVector<double> shiftTimeV[MAX_SCNL];

    /* functions */
    void setup();
    void setupCoeffGraph(QCustomPlot *customPlot, QString, QString);
    void replot();

    void savePick(PICK_INFO *);
    void deleteWidget();

    void writeLogCoeff(COEFF *);
    void writeLogPick(PICK_INFO *);

private slots:  
    /* buttons */
    void playBClicked();
    void hZoomOutBClicked();
    void hZoomInBClicked();
    void scaleBClicked();
    void blinkBClicked();

    /* SLOTs from other's class SIGNALS */
    void rvCoeff(COEFF *);
    void rvPick(PICK_INFO *);
    void recvParamFromConfig(CONFIGURE);
    void rvLocInfo(double lat, double lon, double dist, double azim);

    /* timer SLOTs */
    void makeCCArray();
    void doRepeatWorks();
    void setblink();
    void setSysTime();

    /* actions SLOTs */
    void actionResetClicked();
    void actionEventListClicked();
    void actionConfigureClicked();
    void actionExitClicked(); 
    void actionPrintClicked();
    void actionTempClicked();

    /* label clicked */
    void detailBClicked();
    void viewmbBClicked();

    void showLocAndmb();
};

#endif // MAINWINDOW_H
