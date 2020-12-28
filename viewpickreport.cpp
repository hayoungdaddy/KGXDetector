#include "viewpickreport.h"
#include "ui_viewpickreport.h"

/**
 * @file                viewpickreport.cpp
 *
 * @code
 * EXAMPLE) pick_info
 * 20160909093044.813 1473381044.813
 * KSA HHZ KG -- SPS:100 Threshold:0.60 nRefData:5 pOffset:51
 * P_Arrival:1473381045.313 Max_Amp:94041.00 Per:0.30 Max_AmpTime:1473381045.653 mb:4.54
 * BP1TO6/KG.KSA.HHZ_NK1_REF1n6.mseed:0.378801
 * BP1TO6/KG.KSA.HHZ_NK2_REF1n6.mseed:0.240150
 * BP1TO6/KG.KSA.HHZ_NK3_REF1n6.mseed:0.401205
 * BP1TO6/KG.KSA.HHZ_NK4_REF1n6.mseed:0.633983
 * BP1TO6/KG.KSA.HHZ_NK5_REF1n6.mseed:0.615035
 * @endcode
 */

ViewPickReport::ViewPickReport(QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewPickReport)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QFile file(path + "/pickInfo");

    maxAmpT = -999999;
    minAmpT = 999999;

    nRefData = 0;

    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream(&file);
        QString line;

        line = stream.readLine();
        coeffHTime = line.left(4) + "-" + line.mid(4,2) + "-" + line.mid(6,2) + " " +
                line.mid(8,2) + ":" + line.mid(10,2) + ":" + line.mid(12,2) + "." + line.right(3);
        coeffETime = line.section(" ", 1, 1);

        // SCNL, sps, threshold, pOffset, nRefData, formulaMb
        line = stream.readLine();
        sta = line.section(" ", 0, 0);
        chan = line.section(" ", 1, 1);
        sps = line.section(" ", 4, 4).section(":", 1, 1).toInt();
        delta = (1.0 / sps);
        threshold = line.section(" ", 5, 5).section(":", 1, 1);
        thresholdD = threshold.toDouble();
        nRefData = line.section(" ", 6, 6).section(":", 1, 1).toInt();
        pOffset = line.section(" ", 7, 7).section(":", 1, 1).toInt();

        // p Time, max Amp, period, max Amp Time, delta, nSToMax, mb
        line = stream.readLine();
        pTime = line.section(" ", 0, 0).section(":", 1, 1);
        maxAmp = line.section(" ", 1, 1).section(":",1, 1);
        per = line.section(" ", 2, 2).section(":", 1, 1);
        maxAmpTime = line.section(" ", 3, 3).section(":", 1, 1);
        double x = myRound(maxAmpTime.toDouble(), 3) - myRound(coeffETime.toDouble(), 3);

        nSToMax = myRound((x / delta), 0);

        mb = line.section(" ", 4, 4).section(":", 1, 1);

        for(int i=0;i<nRefData;i++)
        {
            line = stream.readLine();
            if(line.section(":", 1, 1).startsWith("-"))
                coeff[i] = "--";
            else
            {
                double tt = line.section(":", 1, 1).toDouble();
                coeff[i] = QString::number(tt, 'f', NUM_DECIMAL_FOR_COEFF);
            }
        }

        file.close();
    }

    ui->scnl->setText(sta + " " + chan);
    ui->threshold->setText(threshold);
    ui->maxAmp->setText(maxAmp);
    ui->period->setText(per);
    QDateTime dt;
    dt.setTime_t( (uint)maxAmpTime.toDouble() );
    dt = dt.addSecs( 3600 * 9 );
    ui->mTime->setText(dt.toString("yy-MM-dd hh:mm:ss.")+maxAmpTime.section(".",1,1));
    ui->mb->setText(mb);

    dt.setTime_t( (uint)pTime.toDouble() );
    dt = dt.addSecs( 3600 * 9 );
    ui->pTime->setText(dt.toString("yy-MM-dd hh:mm:ss.")+pTime.section(".",1,1));
    ui->coeff1->setText(coeff[0]);
    if(coeff[0].toDouble() >= thresholdD) ui->coeff1->setStyleSheet("background-color: red;");
    ui->coeff2->setText(coeff[1]);
    if(coeff[1].toDouble() >= thresholdD) ui->coeff2->setStyleSheet("background-color: red;");
    ui->coeff3->setText(coeff[2]);
    if(coeff[2].toDouble() >= thresholdD) ui->coeff3->setStyleSheet("background-color: red;");
    ui->coeff4->setText(coeff[3]);
    if(coeff[3].toDouble() >= thresholdD) ui->coeff4->setStyleSheet("background-color: red;");
    ui->coeff5->setText(coeff[4]);
    if(coeff[4].toDouble() >= thresholdD) ui->coeff5->setStyleSheet("background-color: red;");
    ui->coeff6->setText(coeff[5]);
    if(coeff[5].toDouble() >= thresholdD) ui->coeff6->setStyleSheet("background-color: red;");

    setWave(path, ui->wave);
}

ViewPickReport::~ViewPickReport()
{
    delete ui;
}

void ViewPickReport::setWave(QString path, QCustomPlot *customPlot)
{
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    QFile file(path + "/inputData.asc");
    int nData = 0;

    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream(&file);
        QString line;

        line = stream.readLine();
        line = stream.readLine();
        line = stream.readLine();
        line = stream.readLine();

        while(!stream.atEnd())
        {
            line = stream.readLine();
            nData++;
        }
        file.close();
    }

    QVector<double> x(nData), y(nData), min(nData), max(nData);

    int count = 0;

    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream(&file);
        QString line;

        line = stream.readLine();
        line = stream.readLine();
        line = stream.readLine();
        line = stream.readLine();

        while(!stream.atEnd())
        {
            line = stream.readLine();
            y[count] = line.toDouble();
            x[count] = (double)count;
            if(line.toDouble() >= maxAmpT) maxAmpT = line.toDouble();
            if(line.toDouble() <= minAmpT) minAmpT = line.toDouble();
            count++;
        }
        file.close();
    }

    for(int i=0;i<nData;i++)
    {
        min[i] = minAmpT;
        max[i] = maxAmpT;
    }

    QCPGraph *graph = customPlot->addGraph();

    graph->setData(x, y); 
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(100);
    customPlot->xAxis->setLabel("Point");
    customPlot->yAxis->setLabel("Count");
    graph->setPen(QPen(Qt::blue));
    graph->rescaleKeyAxis();
    customPlot->yAxis->setRange(minAmpT, maxAmpT);
    if(maxAmpT <= 10000)
        customPlot->yAxis->setRange(minAmpT-300, maxAmpT+500);
    else if(maxAmpT <= 20000)
        customPlot->yAxis->setRange(minAmpT-3000, maxAmpT+3000);
    else if(maxAmpT <= 50000)
        customPlot->yAxis->setRange(minAmpT-6000, maxAmpT+6000);
    else if(maxAmpT >= 50000)
        customPlot->yAxis->setRange(minAmpT-9000, maxAmpT+9000);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);

    // add the p phase tracer (red circle) which sticks to the graph data (and gets updated in bracketDataSlot by timer event):
    QCPItemTracer *phaseTracer = new QCPItemTracer(customPlot);
    customPlot->addItem(phaseTracer);
    itemDemoPhaseTracer = phaseTracer; // so we can access it later in the bracketDataSlot for animation
    phaseTracer->setGraph(graph);
    phaseTracer->setGraphKey(pOffset-1);
    phaseTracer->setInterpolating(true);
    phaseTracer->setStyle(QCPItemTracer::tsCircle);
    phaseTracer->setPen(QPen(Qt::red));
    phaseTracer->setBrush(Qt::red);
    phaseTracer->setSize(7);

    // add label for p phase tracer:
    QCPItemText *phaseTracerText = new QCPItemText(customPlot);
    customPlot->addItem(phaseTracerText);
    phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
    phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
    phaseTracerText->setText("P Arrival Time:"+pTime);
    phaseTracerText->setTextAlignment(Qt::AlignLeft);
    phaseTracerText->setFont(QFont(font().family(), 9));
    phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

    // add arrow pointing at p phase tracer, coming from label:
    QCPItemCurve *phaseTracerArrow = new QCPItemCurve(customPlot);
    customPlot->addItem(phaseTracerArrow);
    phaseTracerArrow->start->setParentAnchor(phaseTracerText->left);
    phaseTracerArrow->startDir->setParentAnchor(phaseTracerArrow->start);
    phaseTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
    phaseTracerArrow->end->setParentAnchor(phaseTracer->position);
    phaseTracerArrow->end->setCoords(10, 10);
    phaseTracerArrow->endDir->setParentAnchor(phaseTracerArrow->end);
    phaseTracerArrow->endDir->setCoords(30, 30);
    phaseTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
    phaseTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (phaseTracerText->bottom->pixelPoint().y()-phaseTracerText->top->pixelPoint().y())*0.85));

    // add the max amplitude tracer (green circle):
    QCPItemTracer *groupTracer = new QCPItemTracer(customPlot);
    customPlot->addItem(groupTracer);
    groupTracer->setGraph(graph);
    groupTracer->setGraphKey((int)nSToMax);
    groupTracer->setInterpolating(true);
    groupTracer->setStyle(QCPItemTracer::tsCircle);
    groupTracer->setPen(QPen(Qt::green));
    groupTracer->setBrush(Qt::green);
    groupTracer->setSize(7);

    // add label for max amplitud:
    QCPItemText *groupTracerText = new QCPItemText(customPlot);
    customPlot->addItem(groupTracerText);
    groupTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    groupTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignTop);
    groupTracerText->position->setCoords(1.0, 0.01); // lower right corner of axis rect
    groupTracerText->setText("hpp_cnts:"+maxAmp+"\nperiod:"+per+"\npeak time:"+maxAmpTime);
    groupTracerText->setTextAlignment(Qt::AlignLeft);
    groupTracerText->setFont(QFont(font().family(), 9));
    groupTracerText->setPadding(QMargins(8, 0, 0, 0));

    // add arrow pointing at max amplitud, coming from label:
    QCPItemCurve *groupTracerArrow = new QCPItemCurve(customPlot);
    customPlot->addItem(groupTracerArrow);
    groupTracerArrow->start->setParentAnchor(groupTracerText->left);
    groupTracerArrow->startDir->setParentAnchor(groupTracerArrow->start);
    groupTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
    double t = y[(int)nSToMax];
    groupTracerArrow->end->setCoords((int)nSToMax + 1, t);
    groupTracerArrow->endDir->setParentAnchor(groupTracerArrow->end);

    if((int)nSToMax >= (count)/2)
        groupTracerArrow->endDir->setCoords(30, -40);
    else
        groupTracerArrow->endDir->setCoords(10, 0);

    groupTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
    groupTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (groupTracerText->bottom->pixelPoint().y()-groupTracerText->top->pixelPoint().y())*0.85));
}
