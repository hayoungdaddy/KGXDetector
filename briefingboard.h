/**
 * @file                briefingboard.h
 * @brief               BriefingBoard class
 */

#ifndef BRIEFINGBOARD_H
#define BRIEFINGBOARD_H

#include <QDialog>

#include "common.h"
#include "viewpickreport.h"
#include "qcustomplot.h"

namespace Ui {
class BriefingBoard;
}

class BriefingBoard : public QDialog
{
    Q_OBJECT

public:
    explicit BriefingBoard(QString rootDir = 0, QString firstPick = 0, QWidget *parent = 0);
    ~BriefingBoard();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

private:
    Ui::BriefingBoard *ui;

    QLabel *label[5];

    QDateTime et;
    QVector< QVector<QString> > pickList;
    QVector<QString> haveCCpickList;
    QString rDir;

    QLabel *staNameL[MAX_SCNL];
    QLabel *timeL[MAX_SCNL];
    QLabel *coeffL[MAX_SCNL];
    QLabel *mbL[MAX_SCNL];
    QPushButton *detailB[MAX_SCNL];
    QLabel *junk1, *junk2;
    QPushButton *junk3;
    QLabel *avgCoeffL, *avgMbL;

    QCustomPlot *plot[MAX_SCNL];
    QCustomPlot *ccplot;
    QFrame *line;

    QVector<double> ccTime;
    QVector<double> ccData;
    QVector<double> ccThreshold;
    QVector<int> ccNum;
    QVector<QString> staThrValue;

    double ddelta;

    void setup();
    void setupCoeffGraph(QCustomPlot *, QString);
    void drawGraph(QCustomPlot *, QString, QString);
    void getAvg();

private slots:
    void detailBClicked();
    void makeInfo(int);

signals:
    void finished();
};

#endif // BRIEFINGBOARD_H
