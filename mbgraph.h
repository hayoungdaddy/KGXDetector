#ifndef MBGRAPH_H
#define MBGRAPH_H

#include <QDialog>

#include "qcustomplot.h"
#include "common.h"

namespace Ui {
class mbGraph;
}

class mbGraph : public QDialog
{
    Q_OBJECT

public:
    explicit mbGraph(QStringList staNameList, QStringList mbList, QString nkInfoFileName = 0, QWidget *parent = 0);
    ~mbGraph();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

    QString NKInfoFileName;
    double mbAvg;

private:
    Ui::mbGraph *ui;

private slots:
    void nk1Bclicked();
    void nk2Bclicked();
    void nk3Bclicked();
    void nk4Bclicked();
    void nk5Bclicked();
    void nk6Bclicked();

signals:
    void finished();
};

#endif // MBGRAPH_H
