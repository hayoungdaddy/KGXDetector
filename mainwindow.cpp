#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @file                mainwindow.cpp
 */

//MainWindow::MainWindow(QWidget *parent) :
MainWindow::MainWindow(QString configFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* default configure */
    con.pickDir = "/home/yyw/KSRS_EARTHWORM/pickData";
    con.nkinfoFileName = "/home/yyw/KSRS_EARTHWORM/params/NK_Info.txt";
    con.locStanRef = 3;
    con.alarmFileName = "/home/yyw/KSRS_EARTHWORM/params/alarm1.sh";
    con.alarmEnable = 1;
    con.segmentFile = "/home/yyw/KSRS_EARTHWORM/params/dataSegForChina.sh";
    con.gridlatB = 41.225; con.gridlatE = 41.325;
    con.gridlonB = 129.045; con.gridlonE = 129.145;

    // KSA = 45, YKB = 51.728. CHNB = 54.822

    con.staName[0] = "KSA"; con.chanName[0] = "HHZ"; con.threshold[0] = 0.6; con.ccEnable[0] = 1; con.timeShift[0] = 45;
    con.staName[1] = "YKB"; con.chanName[1] = "HHZ"; con.threshold[1] = 0.6; con.ccEnable[1] = 1; con.timeShift[1] = 51.73;
    con.staName[2] = "CHNB"; con.chanName[2] = "HHZ"; con.threshold[2] = 0.6; con.ccEnable[2] = 1; con.timeShift[2] = 54.82;
    con.staName[3] = "BRD"; con.chanName[3] = "HHZ"; con.threshold[3] = 0.6; con.ccEnable[3] = 0; con.timeShift[3] = 0;
    con.staName[4] = "KS31"; con.chanName[4] = "BHZ"; con.threshold[4] = 0.6; con.ccEnable[4] = 0; con.timeShift[4] = 0;
    con.staName[5] = "USA0B"; con.chanName[5] = "BHZ"; con.threshold[5] = 0.6; con.ccEnable[5] = 0; con.timeShift[5] = 0;
    con.staName[6] = "DNH"; con.chanName[6] = "HHZ"; con.threshold[6] = 0.6; con.ccEnable[6] = 0; con.timeShift[6] = 0;
    con.staName[7] = "NSN"; con.chanName[7] = "HHZ"; con.threshold[7] = 0.6; con.ccEnable[7] = 0; con.timeShift[7] = 0;
    con.staName[8] = "YNB"; con.chanName[8] = "HHZ"; con.threshold[8] = 0.6; con.ccEnable[8] = 0; con.timeShift[8] = 0;
    con.staName[9] = "YNG"; con.chanName[9] = "HHZ"; con.threshold[9] = 0.6; con.ccEnable[9] = 0; con.timeShift[9] = 0;
    con.ccSps = 100;
/*
    con.staName[0] = "KSA"; con.chanName[0] = "BHZ"; con.threshold[0] = 0.6; con.ccEnable[0] = 1; con.timeShift[0] = 45;
    con.staName[1] = "YKB"; con.chanName[1] = "BHZ"; con.threshold[1] = 0.6; con.ccEnable[1] = 1; con.timeShift[1] = 51.78;
    con.staName[2] = "CHNB"; con.chanName[2] = "BHZ"; con.threshold[2] = 0.6; con.ccEnable[2] = 1; con.timeShift[2] = 54.82;
    con.staName[3] = "BRD"; con.chanName[3] = "BHZ"; con.threshold[3] = 0.6; con.ccEnable[3] = 0; con.timeShift[3] = 0;
    con.staName[4] = "KS31"; con.chanName[4] = "BHZ"; con.threshold[4] = 0.6; con.ccEnable[4] = 0; con.timeShift[4] = 0;
    con.staName[5] = "USA0B"; con.chanName[5] = "BHZ"; con.threshold[5] = 0.6; con.ccEnable[5] = 0; con.timeShift[5] = 0;
    con.staName[6] = "DNH"; con.chanName[6] = "BHZ"; con.threshold[6] = 0.6; con.ccEnable[6] = 0; con.timeShift[6] = 0;
    con.staName[7] = "NSN"; con.chanName[7] = "BHZ"; con.threshold[7] = 0.6; con.ccEnable[7] = 0; con.timeShift[7] = 0;
    con.staName[8] = "YNB"; con.chanName[8] = "BHZ"; con.threshold[8] = 0.6; con.ccEnable[8] = 0; con.timeShift[8] = 0;
    con.staName[9] = "YNG"; con.chanName[9] = "BHZ"; con.threshold[9] = 0.6; con.ccEnable[9] = 0; con.timeShift[9] = 0;
    con.ccSps = 20;
*/
    con.NccEnable = 3;

    con.ccthreshold = 0.6;
    con.zmqServerIP = "127.0.0.1";
    con.zmqPortForPick = 5555;
    con.zmqPortForCoeff = 5556;
    con.stanRef = 0;

    QFile file(configFile);
    if(!file.exists())
    {
        qDebug() << "Parameters file don't exists.";
        exit(1);
    }
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line, _line;

        while(!stream.atEnd())
        {
            line = stream.readLine();
            _line = line.simplified();

            if(_line.startsWith("PickDir"))
                con.pickDir = _line.section("=", 1, 1);
            else if(_line.startsWith("NKInfoFileName"))
                con.nkinfoFileName = _line.section("=", 1, 1);
            else if(_line.startsWith("StandardReference"))
            {
                if(_line.section("=", 1, 1).startsWith("ALL"))
                    con.stanRef = 0;
                else if(_line.section("=", 1, 1).startsWith("NK1"))
                    con.stanRef = 1;
                else if(_line.section("=", 1, 1).startsWith("NK2"))
                    con.stanRef = 2;
                else if(_line.section("=", 1, 1).startsWith("NK3"))
                    con.stanRef = 3;
                else if(_line.section("=", 1, 1).startsWith("NK4"))
                    con.stanRef = 4;
                else if(_line.section("=", 1, 1).startsWith("NK5"))
                    con.stanRef = 5;
            }
            else if(_line.startsWith("LocationStandardReference"))
            {
                if(_line.section("=", 1, 1).startsWith("NK1"))
                    con.locStanRef = 0;
                else if(_line.section("=", 1, 1).startsWith("NK2"))
                    con.locStanRef = 1;
                else if(_line.section("=", 1, 1).startsWith("NK3"))
                    con.locStanRef = 2;
                else if(_line.section("=", 1, 1).startsWith("NK4"))
                    con.locStanRef = 3;
                else if(_line.section("=", 1, 1).startsWith("NK5"))
                    con.locStanRef = 4;
            }
            else if(_line.startsWith("AlarmScript"))
                con.alarmFileName = _line.section("=", 1, 1);
            else if(_line.startsWith("SegmentScript"))
                con.segmentFile = _line.section("=", 1, 1);
            else if(_line.startsWith("GridLatitude"))
            {
                con.gridlatB = _line.section("=", 1, 1).section("/", 0, 0).toDouble();
                con.gridlatE = _line.section("=", 1, 1).section("/", 1, 1).toDouble();
            }
            else if(_line.startsWith("GridLongitude"))
            {
                con.gridlonB = _line.section("=", 1, 1).section("/", 0, 0).toDouble();
                con.gridlonE = _line.section("=", 1, 1).section("/", 1, 1).toDouble();
            }
            else if(_line.startsWith("ThresholdForCC"))
                con.ccthreshold = _line.section("=", 1, 1).toFloat();
            else if(_line.startsWith("ZmqServerIP"))
                con.zmqServerIP = _line.section("=", 1, 1);
            else if(_line.startsWith("ZmqServerIP"))
                con.zmqPortForPick = _line.section("=", 1, 1).toInt();
            else if(_line.startsWith("ZmqServerIP"))
                con.zmqPortForCoeff = _line.section("=", 1, 1).toInt();
        }
        file.close();
    }

    /* thread */
    rvCoeffThread = new QThread(this);
    rvPickThread = new QThread(this);
    getFinalLocThread = new QThread(this);

    /* widget */
    config = new Config();
    config->hide();

    sohTimer = new QTimer(this);
    sohTimer->start(TIME_INTERVAL);
    connect(sohTimer, SIGNAL(timeout()), this, SLOT(doRepeatWorks()));

    ccTimer = new QTimer(this);
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(setblink()));
    sysTimer = new QTimer(this); sysTimer->start(1000);
    connect(sysTimer, SIGNAL(timeout()), this, SLOT(setSysTime()));

    setup();

    connect(config, SIGNAL(sendParamToMainWindow(CONFIGURE)), this, SLOT(recvParamFromConfig(CONFIGURE)));

    /* action connects */
    connect(ui->actionEventList, SIGNAL(triggered(bool)), this, SLOT(actionEventListClicked()));
    connect(ui->actionConfigure, SIGNAL(triggered(bool)), this, SLOT(actionConfigureClicked()));
    connect(ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(actionResetClicked()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(actionExitClicked()));
    connect(ui->actionPrint, SIGNAL(triggered(bool)), this, SLOT(actionPrintClicked()));
    //connect(ui->actionTemp, SIGNAL(triggered(bool)), this, SLOT(actionTempClicked()));

    /* button connects */
    connect(ui->playB, SIGNAL(clicked(bool)), this, SLOT(playBClicked()));
    connect(ui->hZoomOutB, SIGNAL(clicked(bool)), this, SLOT(hZoomOutBClicked()));
    connect(ui->hZoomInB, SIGNAL(clicked(bool)), this, SLOT(hZoomInBClicked()));
    connect(ui->scaleB, SIGNAL(clicked(bool)), this, SLOT(scaleBClicked()));
    connect(ui->blinkB, SIGNAL(clicked(bool)), this, SLOT(blinkBClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup()
{
    QFont f1( "Sans", 13, QFont::Normal);
    QFont f2( "Sans", 13, QFont::Bold);
    QFont f3( "Sans", 10, QFont::Bold);

    //ui->azimL->setText("Azim.(with NK" + QString::number(con.locStanRef + 1) + ")");
    ui->locLabel->setText("Location Info. (Ref. : NK" + QString::number(con.locStanRef + 1) +")");

    /* default variables */
    isPlay = 1;
    //xSizeArray = {60, 120, 300, 600}; // 1m, 2m, 5m, 10m
    xSizeArray[0] = 60; xSizeArray[1] = 120; xSizeArray[2] = 300; xSizeArray[3] = 600;
    xSizeId = 0;
    ySizeId = 0;

    ui->lat->setText("");
    ui->lon->setText("");
    ui->dist->setText("");
    ui->azim->setText("");

    int i;

    QLabel *label[5];
    for(i=0;i<5;i++)
    {
        label[i] = new QLabel(this);
        label[i]->setFont(f1);
        label[i]->setAlignment(Qt::AlignCenter);
        ui->gridLayout->addWidget(label[i], 0, i);
    }

    label[0]->setText("Station");
    label[1]->setText("Time(KST)");
    label[2]->setText("Coefficient");
    label[3]->setText("mb(ARS)");
    label[4]->setText("Detail View");

    mmap.clear();

    event.isEvent = 0;
    event.blink = 0;
    event.isGettingLoc = 0;

    ccPlot = new QCustomPlot;
    ui->graphLayout->addWidget(ccPlot);
    setupCoeffGraph(ccPlot, "Cross Correlograms", "DarkBlue");

    line = new QFrame();
    line->setGeometry(QRect(/* ... */));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Plain);
    line->setStyleSheet("background-color: rgb(71, 107, 41);");
    ui->graphLayout->addWidget(line);

    double sum_timeShift = 0;
    for(i=0;i<MAX_SCNL;i++)
    {
        if(con.ccEnable[i] == 1)
            sum_timeShift = sum_timeShift + con.timeShift[i];

        event.eventSta[i] = con.staName[i];
        event.firstEventTime[i] = 0;
        event.eventTimeForLoc[i] = 0;
        event.isCompleted[i] = 0;

        maxCoeffV[i].clear();
        shiftTimeV[i].clear();

        con.lastTime[i] = 0;
        con.isAlived[i] = -1;

        staNameL[i] = new QLabel;
        staNameL[i]->setText(con.staName[i]);
        staNameL[i]->setObjectName(con.chanName[i]);
        staNameL[i]->setFrameStyle(1);
        staNameL[i]->setAlignment(Qt::AlignCenter);
        staNameL[i]->setFont(f2);
        staNameL[i]->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
        timeL[i] = new QLabel;
        timeL[i]->setFrameStyle(1);
        timeL[i]->setAlignment(Qt::AlignCenter);
        timeL[i]->setStyleSheet("padding: 1px 8px 1px 8px;");
        timeL[i]->setFont(f1);
        coeffL[i] = new QLabel;
        coeffL[i]->setFrameStyle(1);
        coeffL[i]->setAlignment(Qt::AlignCenter);
        coeffL[i]->setFont(f1);
        mbL[i] = new QLabel;
        mbL[i]->setFrameStyle(1);
        mbL[i]->setAlignment(Qt::AlignCenter);
        mbL[i]->setFont(f1);

        detailB[i] = new QPushButton;
        detailB[i]->setFont(f1);
        detailB[i]->setObjectName("detailB" + QString::number(i));
        detailB[i]->setStyleSheet("border-style:solid;border-color: black;border-width:1px;");

        if(staNameL[i]->text() != "")
        {
            ui->gridLayout->addWidget(staNameL[i], i+1, 0);
            ui->gridLayout->addWidget(timeL[i], i+1, 1);
            ui->gridLayout->addWidget(coeffL[i], i+1, 2);
            ui->gridLayout->addWidget(mbL[i], i+1, 3);
            ui->gridLayout->addWidget(detailB[i], i+1, 4);
            connect(detailB[i], SIGNAL(clicked(bool)), this, SLOT(detailBClicked()));
        }

        if(con.ccEnable[i] == 1)
        {
            plot[i] = new QCustomPlot;
            ui->graphLayout->addWidget(plot[i]);
            setupCoeffGraph(plot[i], con.staName[i], "DarkCyan");
        }
    }

    ccLastTime = 0;

    /* if you want tankplyer mode, then you might be block below */
    QDateTime st = QDateTime::currentDateTimeUtc();
    ccLastTime = st.toTime_t() - (int)(sum_timeShift / con.NccEnable);
    /*************************************************************/

    myLayout = new QHBoxLayout;
    ui->gridLayout->addLayout(myLayout, i+1, 0);
    legend1 = new QLabel; legend1->setText("< 30s"); legend1->setAlignment(Qt::AlignCenter); legend1->setFont(f3);
    legend1->setStyleSheet("background-color: rgb(53, 97, 24);color: rgb(255, 255, 255);");
    legend2 = new QLabel; legend2->setText("< 60s"); legend2->setAlignment(Qt::AlignCenter); legend2->setFont(f3);
    legend2->setStyleSheet("background-color: rgb(175, 84, 33);color: rgb(255, 255, 255);");
    legend3 = new QLabel; legend3->setText("> 60s"); legend3->setAlignment(Qt::AlignCenter); legend3->setFont(f3);
    legend3->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
    myLayout->addWidget(legend1);
    myLayout->addWidget(legend2);
    myLayout->addWidget(legend3);

    avgLabel = new QLabel; avgLabel->setText("Average"); avgLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->gridLayout->addWidget(avgLabel, i+1, 1);
    avgCoeffL = new QLabel; avgCoeffL->setFrameStyle(1); avgCoeffL->setAlignment(Qt::AlignCenter); avgCoeffL->setFont(f1);
    ui->gridLayout->addWidget(avgCoeffL, i+1, 2);
    avgMbL = new QLabel; avgMbL->setFrameStyle(1); avgMbL->setAlignment(Qt::AlignCenter); avgMbL->setFont(f1);
    ui->gridLayout->addWidget(avgMbL, i+1, 3);
    viewmbB = new QPushButton; viewmbB->setFont(f1);
    viewmbB->setStyleSheet("border-style:solid;border-color: black;border-width:1px;");
    //viewmbB->setText("View mb Graph");
    ui->gridLayout->addWidget(viewmbB, i+1, 4);
    connect(viewmbB, SIGNAL(clicked(bool)), this, SLOT(viewmbBClicked()));

    rvcoeffzmq = new RvCoeffzmq();
    rvcoeffzmq->setup(con.zmqServerIP, con.zmqPortForCoeff);
    rvcoeffzmq->moveToThread(rvCoeffThread);
    rvCoeffThread->start();

    rvpickzmq = new RvPickzmq();
    rvpickzmq->setup(con.zmqServerIP, con.zmqPortForPick);
    rvpickzmq->moveToThread(rvPickThread);
    rvPickThread->start();

    ccTimer->start(TIME_INTERVAL);
    connect(ccTimer, SIGNAL(timeout()), this, SLOT(makeCCArray()));

    connect(rvcoeffzmq, SIGNAL(sendCOEFFtoMain(COEFF*)), this, SLOT(rvCoeff(COEFF*)));
    connect(rvpickzmq, SIGNAL(sendPICKtoMain(PICK_INFO*)), this, SLOT(rvPick(PICK_INFO*)));
}

void MainWindow::playBClicked()
{
    if(isPlay == 1)
    {
        ui->playB->setText("Play");
        isPlay = 0;
        ccTimer->stop();
    }
    else if(isPlay == 0)
    {
        ui->playB->setText("Pause");
        isPlay = 1;
        ccTimer->start(TIME_INTERVAL);
    }
}

void MainWindow::rvCoeff(COEFF *coeff)
{
    QString thisSta, thisChan;
    thisSta = QString(coeff->sta);
    thisChan = QString(coeff->chan);
    double thisStartTime = myRound(coeff->startTime, 2);
    double thisDelta = myRound((1.0 / coeff->sps), 2);
    int thisNsamp = coeff->nSamp;
    int NsearchSamp = 0;

    if(con.ccSps == 100)
        NsearchSamp = FIND_MAX_COEFF_RANGE_FOR_100;
    else if(con.ccSps == 20)
        NsearchSamp = FIND_MAX_COEFF_RANGE_FOR_20;

    for(int i=0;i<MAX_SCNL;i++)
    {
        /* get last time */
        if(con.staName[i].startsWith(thisSta) && con.chanName[i].startsWith(thisChan))
        {
            if(thisStartTime < con.lastTime[i])
                break;
            con.lastTime[i] = thisStartTime + (thisDelta * thisNsamp);
            break;
        }
    }

    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.staName[i].startsWith(thisSta) && con.chanName[i].startsWith(thisChan) && con.ccEnable[i] == 1)
        {
            writeLogCoeff(coeff);
            double mTime = thisStartTime - con.timeShift[i];
            if(con.ccSps == 100) mTime = myRound(mTime, 2);
            else if(con.ccSps == 20)
            {
                QString temp;
                temp = temp.setNum(mTime, 'f', 2);
                if(temp.right(1) == "0" || temp.right(1) == "1" || temp.right(1) == "2" || temp.right(1) == "3" || temp.right(1) == "4")
                    temp = temp.left(12) + "0";
                else if(temp.right(1) == "5" || temp.right(1) == "6" || temp.right(1) == "7" || temp.right(1) == "8" || temp.right(1) == "9")
                    temp = temp.left(12) + "5";
                mTime = temp.toDouble();
            }

            for(int j=0;j<thisNsamp;j++)
            {
                maxCoeffV[i].push_back((double)coeff->maxCoeffArray[j]);
                shiftTimeV[i].push_back(myRound(mTime + (thisDelta * j), 2));
            }

            if(maxCoeffV[i].count() > (NsearchSamp * 2) + 1)
            {
                QVector<double> time, data, threshold;
                COEFFMAP coeffmap;

                for(int j=NsearchSamp;j<maxCoeffV[i].count()-NsearchSamp;j++)
                {
                    double max = 0;
                    for(int k=j-NsearchSamp;k<=j+NsearchSamp;k++)
                    {
                        if(maxCoeffV[i].at(k) > max)
                            max = maxCoeffV[i].at(k);
                    }

                    data.push_back(max);
                    time.push_back(myRound(shiftTimeV[i].at(j), 2));
                    threshold.push_back(con.threshold[i]);

                    coeffmap.staName = con.staName[i];
                    coeffmap.coeff = max;
                    mmap.insert(myRound(shiftTimeV[i].at(j), 2), coeffmap);
                }

                plot[i]->graph(0)->addData(time, data);
                plot[i]->graph(1)->addData(time, threshold);
                plot[i]->graph(0)->removeDataBefore(time.last() - MAX_KEEP_TIME);
                plot[i]->graph(1)->removeDataBefore(time.last() - MAX_KEEP_TIME);

                maxCoeffV[i].remove(0, maxCoeffV[i].count()-(NsearchSamp*2));
                shiftTimeV[i].remove(0, shiftTimeV[i].count()-(NsearchSamp*2));
            }
            break;
        }
    }
}

void MainWindow::makeCCArray()
{
    QVector<double> ccTime, ccData, ccThreshold;

    QMultiMap<double, COEFFMAP>::iterator iter;

    // all ccEnabled stations are alived
    int isAllDead = 0; int realNccEnable = con.NccEnable;

    // realtime mode. if you want tankplayer mode, then you might be block below
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.ccEnable[i] == 1 && con.isAlived[i] != 1)
            realNccEnable--;
    }
    if(realNccEnable == 0) isAllDead = 1;
    /***************************************************************************/

    if(isAllDead == 0)
    {
        double key = 0;
        for(iter = mmap.end(); iter != mmap.begin(); iter--)
        {
            if(iter != mmap.end() && mmap.count(iter.key()) == realNccEnable)
            {
                key = iter.key();
                break;
            }
        }

        if(iter != mmap.begin())
        {
            for(iter = mmap.upperBound(ccLastTime); iter.key() <= key;)
            {
                if(iter != mmap.end()) iter++;
                else break;

                QPair<QMultiMap<double, COEFFMAP>::iterator, QMultiMap<double, COEFFMAP>::iterator> iter_pair;
                iter_pair = mmap.equal_range(iter.key());
                QMultiMap<double, COEFFMAP>::iterator nowIter;
                int count = 0; double sum = 0;
                for( nowIter = iter_pair.first ; nowIter != iter_pair.second ; nowIter++)
                {
                    COEFFMAP coeffmap = nowIter.value();
                    count++; sum = sum + coeffmap.coeff;
                }

                if(count != 0)
                {
                    ccTime.push_back(iter.key());
                    ccData.push_back(sum / count);
                    if( sum / count >= con.ccthreshold)
                        blinkTimer->start(500);

                    ccThreshold.push_back(con.ccthreshold);
                }
            }
        }
    }
    else
    {
        ccLastTime = ccLastTime + (TIME_INTERVAL / 1000);
        replot();
    }

    if(!ccTime.isEmpty())
    {
        ccPlot->graph(0)->addData(ccTime, ccData);
        ccPlot->graph(1)->addData(ccTime, ccThreshold);
        ccPlot->graph(0)->removeDataBefore(ccTime.last() - MAX_KEEP_TIME);
        ccPlot->graph(1)->removeDataBefore(ccTime.last() - MAX_KEEP_TIME);
        ccLastTime = ccTime.last();
        replot();
    }

    // delete mmap
    if(!mmap.isEmpty())
    {
        if(mmap.lastKey() - mmap.firstKey() >= MAX_KEEP_TIME)
        {
            QMultiMap<double, COEFFMAP>::iterator untilIter;
            untilIter = mmap.lowerBound(mmap.lastKey() - MAX_KEEP_TIME);

            for(iter = mmap.begin() ; untilIter != iter ;)
            {
                QMultiMap<double, COEFFMAP>::iterator thisIter;
                thisIter = iter;
                iter++;
                mmap.erase(thisIter);
            }
        }
    }

    /* save ccArray to file when event */
    /* save -30 ~ first Event Time ~ + 30 */
    if(event.isEvent == 1)
    {
        for(int i=0;i<MAX_SCNL;i++)
        {
            if(event.isCompleted[i] != 1 && event.firstEventTime[i] != 0 && con.ccEnable[i] == 1)
            {
                double mTime = event.firstEventTime[i] - HALF_BBOARD_GRAPH_LENGTH - con.timeShift[i];
                if(con.ccSps == 100)
                    mTime = myRound(mTime, 2);
                else if(con.ccSps == 20)
                {
                    QString temp;
                    temp = temp.setNum(mTime, 'f', 2);
                    if(temp.right(1) == "0" || temp.right(1) == "1" || temp.right(1) == "2" || temp.right(1) == "3" || temp.right(1) == "4")
                        temp = temp.left(12) + "0";
                    else if(temp.right(1) == "5" || temp.right(1) == "6" || temp.right(1) == "7" || temp.right(1) == "8" || temp.right(1) == "9")
                        temp = temp.left(12) + "5";
                    mTime = temp.toDouble();
                }

                for(iter = mmap.end(); iter != mmap.begin(); iter--)
                {
                    if(iter != mmap.end())
                    {
                        COEFFMAP coeffmap = iter.value();
                        if(coeffmap.staName.startsWith(event.eventSta[i]))
                            break;
                    }
                }

                if(iter != mmap.begin() && iter.key() >= mTime + (HALF_BBOARD_GRAPH_LENGTH * 2))
                {
                    QVector<double> ccArray; int count = 0; double firstKey = 0;
                    for(iter = mmap.lowerBound(mTime); count < con.ccSps*HALF_BBOARD_GRAPH_LENGTH*2 ; iter++)
                    {
                        COEFFMAP coeffmap = iter.value();
                        if(coeffmap.staName.startsWith(event.eventSta[i]))
                        {
                            if(count == 0) firstKey = iter.key();
                            ccArray.push_back(coeffmap.coeff);
                            count++;
                        }
                    }

                    QDateTime dt;
                    dt.setTime_t( (uint)event.firstEventTime[i]);
                    dt = dt.addSecs( 3600 * 9 );
                    QString rootDir = con.pickDir + "/" + dt.toString("yyyy") + "/" + dt.toString("yyyyMMddhhmmss") + "." +
                            QString::number(event.firstEventTime[i], 'f', 3).section(".", 1, 1) + "/ccArray";
                    QDir dir(rootDir); if(!dir.exists()) dir.mkpath(".");
                    QFile file(rootDir + "/" + event.eventSta[i]);
                    if(file.open(QIODevice::WriteOnly))
                    {
                        QTextStream stream( &file );

                        stream << QString::number(firstKey, 'f', 2) << "\n";

                        for(int j=0;j<ccArray.count();j++)
                            stream << QString::number(ccArray.at(j), 'f', 6) << "\n";

                        event.isCompleted[i] = 1;
                        file.close();
                    }
                }
            }
        }
    }
    /***********************************/
}

void MainWindow::doRepeatWorks()
{
    /* for SOH */
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.lastTime[i] != 0)
        {
            QDateTime dt;
            dt.setTime_t( (uint)con.lastTime[i] );
            int diff = systemTime.toTime_t() - dt.toTime_t();

            if(diff <= SOH_GREEN)
            {
                con.isAlived[i] = 1;
                staNameL[i]->setStyleSheet("background-color: rgb(53, 97, 24);color: rgb(255, 255, 255);");
            }
            else if(diff > SOH_GREEN && diff <= SOH_YELLOW)
            {
                con.isAlived[i] = 0;
                staNameL[i]->setStyleSheet("background-color: rgb(175, 84, 33);color: rgb(255, 255, 255);");
            }
            else
            {
                con.isAlived[i] = -1;
                staNameL[i]->setStyleSheet("background-color : red;color: rgb(255, 255, 255);");
            }
        }
        else
        {
            con.isAlived[i] = -1;
            staNameL[i]->setStyleSheet("background-color : red;color: rgb(255, 255, 255);");
        }
    }

    // get location
    int nCountForLoc = 0;
    QVector<QString> staList;
    QVector<double> pArrivalTime;
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(event.eventTimeForLoc[i] != 0)
        {
            nCountForLoc++;
            staList.push_back(event.eventSta[i]);
            pArrivalTime.push_back(event.eventTimeForLoc[i]);
        }
    }

    if(nCountForLoc >= MINIMUM_SCNL_FOR_LOC && event.isGettingLoc == 0)
    {
        event.isGettingLoc = 1;

        CallLocationNoGUI *calllocationnogui = new CallLocationNoGUI();
        calllocationnogui->makeGrid(con.gridlatB, con.gridlatE, con.gridlonB, con.gridlonE, GRID_SPACING);
        calllocationnogui->ready(con.pickDir, con.locStanRef, con.nkinfoFileName);
        calllocationnogui->setup(staList, pArrivalTime);

        QThread *tempThread = new QThread();
        calllocationnogui->moveToThread(tempThread);
        tempThread->start();
        connect(tempThread, SIGNAL(started()), calllocationnogui, SLOT(getLoc()));

        connect(calllocationnogui, SIGNAL(finished(double, double, double, double)), this, SLOT(rvLocInfo(double, double, double, double)));
        connect(calllocationnogui, SIGNAL(killme()), calllocationnogui, SLOT(deleteLater()));
        connect(calllocationnogui, SIGNAL(destroyed()), tempThread, SLOT(quit()));
        connect(tempThread, SIGNAL(finished()), tempThread, SLOT(deleteLater()));
    }

    // init. event
    if(event.isEvent == 1)
    {
        int t1, t2;
        t1 = systemTime.toTime_t(); t2 = event.systemTimeWhenEvent.toTime_t();

        if(t1 - t2 > MAX_TIME_GAP)
        {
            event.isEvent = 0;
            event.isGettingLoc = 0;
            double minTime = 0;

            for(int i=0;i<MAX_SCNL;i++)
            {
                if(event.firstEventTime[i] != 0)
                {           
                    if(minTime == 0)
                        minTime = event.firstEventTime[i];
                    if(minTime > event.firstEventTime[i])
                        minTime = event.firstEventTime[i];
                }
            }

            // get final location
            if(minTime != 0 )
            {
                //qDebug() << "start getting location";
                CallLocation *calllocation = new CallLocation();
                QThread *tempThread = new QThread();
                calllocation->moveToThread(tempThread);

                calllocation->setup(con.pickDir, con.locStanRef, minTime, con.nkinfoFileName,
                                    con.gridlatB, con.gridlatE, con.gridlonB, con.gridlonE);

                tempThread->start();
                connect(tempThread, SIGNAL(started()), calllocation, SLOT(getLoc()));

                connect(calllocation, SIGNAL(finished()), calllocation, SLOT(deleteLater()));
                connect(calllocation, SIGNAL(destroyed()), tempThread, SLOT(quit()));
                connect(tempThread, SIGNAL(finished()), tempThread, SLOT(deleteLater()));
                connect(tempThread, SIGNAL(destroyed()), this, SLOT(showLocAndmb()));
            }

            for(int i=0;i<MAX_SCNL;i++)
            {
                event.eventSta[i] = con.staName[i];
                //event.firstEventTime[i] = 0;
                event.eventTimeForLoc[i] = 0;
                event.coeffForLoc[i] = 0;
                event.isCompleted[i] = 0;
            }
        }
    }
}

void MainWindow::rvLocInfo(double lat, double lon, double dist, double azim)
{
    event.isGettingLoc = 0;
    ui->lat->setText(QString::number(lat, 'f', 4));
    ui->lon->setText(QString::number(lon, 'f', 4));
    ui->dist->setText(QString::number(dist, 'f', NUM_DECIMAL_FOR_DIST));
    ui->azim->setText(QString::number(azim, 'f', NUM_DECIMAL_FOR_AZIM));
}

void MainWindow::showLocAndmb()
{
    double minTime = 0;

    for(int i=0;i<MAX_SCNL;i++)
    {
        if(event.firstEventTime[i] != 0)
        {
            if(minTime == 0)
                minTime = event.firstEventTime[i];
            if(minTime > event.firstEventTime[i])
                minTime = event.firstEventTime[i];
        }
    }

    QStringList staList, mbList;
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(mbL[i]->text() != "")
        {
            staList << staNameL[i]->text();
            mbList << mbL[i]->text();
        }
    }

    QDateTime tt;
    tt.setTime_t(minTime);
    tt = tt.addSecs( 3600 * 9 );

    /*
    LocGraph *locgraph = new LocGraph(con.pickDir, tt.toString("yyyyMMddhhmmss"), con.nkinfoFileName, con.locStanRef);
    locgraph->show();

    connect(locgraph, SIGNAL(finished()), locgraph, SLOT(deleteLater()));

    mbGraph *mbgraph = new mbGraph(staList, mbList, con.nkinfoFileName);
    mbgraph->show();

    connect(mbgraph, SIGNAL(finished()), mbgraph, SLOT(deleteLater()));
    */

    //qDebug() << QString::number(minTime, 'f', 3);

    for(int i=0;i<MAX_SCNL;i++)
        event.firstEventTime[i] = 0;
}

void MainWindow::setblink()
{
    if(event.blink == 0)
    {
        ui->ccBox->setStyleSheet("background-color : red;");
        event.blink = 1;
    }
    else if(event.blink == 1)
    {
        ui->ccBox->setStyleSheet("background-color : white;");
        event.blink = 0;
    }
}

void MainWindow::setSysTime()
{
    systemTime = QDateTime::currentDateTimeUtc();
    QDateTime st = QDateTime::currentDateTimeUtc();
    st = st.addSecs( 3600 * 9 ) ;
    ui->sysTime->display(st.toString("yyyy-MM-dd hh:mm:ss"));
    st = st.addSecs( -(60 * 30) );
    ui->pyTime->display(st.toString("yyyy-MM-dd hh:mm:ss"));
    QDateTime ut = QDateTime::currentDateTimeUtc();
    ui->utcTime->display(ut.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::rvPick(PICK_INFO *pick)
{
    QString thisSta, thisChan, msecS;
    thisSta = QString(pick->sta);
    thisChan = QString(pick->chan);
    msecS = QString::number(pick->coeffTime, 'f', 3).section(".", 1, 1);
    QDateTime ct;
    ct.setTime_t((int)pick->coeffTime);
    ct = ct.addMSecs(msecS.toInt());

    /* save data */
    writeLogPick(pick);
    savePick(pick);

    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.staName[i].startsWith(thisSta) && con.chanName[i].startsWith(thisChan))
        {
            /* check event */
            if(event.isEvent == 0)
            {
                if(con.alarmEnable == 1)
                {
                    QString temp;
                    QString temp2 = QString::number(pick->coeffTime, 'f', 3);
                    temp2 = temp2.section(".", 0, 0);
                    //temp = con.alarmFileName + " &";
                    temp = con.alarmFileName + " " + con.staName[i] + " " + temp2 + " &";
                    system(temp.toLatin1().data());

                    temp = con.segmentFile + " " + con.staName[i] + " " + temp2 + " &";
                    system(temp.toLatin1().data());
                }

                event.systemTimeWhenEvent = QDateTime::currentDateTimeUtc();
                event.isEvent = 1;

                event.firstEventTime[i] = pick->coeffTime;
            }
            else if(event.isEvent == 1 && event.firstEventTime[i] == 0)
                event.firstEventTime[i] = pick->coeffTime;

            if(event.eventTimeForLoc[i] == 0 && pick->eachCoeff[con.locStanRef] > 0)
            {
                event.eventTimeForLoc[i] = pick->pArrivalTime;
                event.coeffForLoc[i] = pick->eachCoeff[con.locStanRef]; // 0-NK1, 1-NK2, 2-NK3, 3-NK4, 4-NK5
            }
            else if(event.eventTimeForLoc[i] != 0)
            {
                if(pick->eachCoeff[con.locStanRef] > event.coeffForLoc[i])
                {
                    event.eventTimeForLoc[i] = pick->pArrivalTime;
                    event.coeffForLoc[i] = pick->eachCoeff[con.locStanRef];
                }
            }
            /************************/

            ct = ct.addSecs( 3600 * 9 );
            if(coeffL[i]->text() == "")
            {
                if(con.stanRef == 0)
                {
                    timeL[i]->setText(ct.toString("yyyy-MM-dd hh:mm:ss.zzz"));
                    coeffL[i]->setText(QString::number(pick->maxCoeff, 'f', NUM_DECIMAL_FOR_COEFF));
                    mbL[i]->setText(QString::number(pick->mb, 'f', 2));
                    detailB[i]->setText("View");
                }
                else
                {
                    if(pick->eachCoeff[con.stanRef-1] >= 0)
                    {
                        timeL[i]->setText(ct.toString("yyyy-MM-dd hh:mm:ss.zzz"));
                        coeffL[i]->setText(QString::number(pick->eachCoeff[con.stanRef-1], 'f', NUM_DECIMAL_FOR_COEFF));
                        mbL[i]->setText(QString::number(pick->mb, 'f', 2));
                        detailB[i]->setText("View");
                    }
                }
            }
            else
            {
                if(con.stanRef == 0)
                {
                    if(pick->maxCoeff > coeffL[i]->text().toFloat())
                    {
                        timeL[i]->setText(ct.toString("yyyy-MM-dd hh:mm:ss.zzz"));
                        coeffL[i]->setText(QString::number(pick->maxCoeff, 'f', NUM_DECIMAL_FOR_COEFF));
                        mbL[i]->setText(QString::number(pick->mb, 'f', 2));
                        detailB[i]->setText("View");
                    }
                }
                else
                {
                    if(pick->eachCoeff[con.stanRef-1] > coeffL[i]->text().toFloat())
                    {
                        timeL[i]->setText(ct.toString("yyyy-MM-dd hh:mm:ss.zzz"));
                        coeffL[i]->setText(QString::number(pick->eachCoeff[con.stanRef-1], 'f', NUM_DECIMAL_FOR_COEFF));
                        mbL[i]->setText(QString::number(pick->mb, 'f', 2));
                        detailB[i]->setText("View");
                    }
                }
            }
            break;
        }
    }

    int count = 0;
    float sum = 0;
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(coeffL[i]->text() != "")
        {
            sum += coeffL[i]->text().toFloat();
            count++;
        }
    }
    if(sum != 0) avgCoeffL->setText(QString::number(sum / (float)count, 'f', NUM_DECIMAL_FOR_COEFF));

    count = 0; sum = 0;
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(mbL[i]->text() != "")
        {
            sum += mbL[i]->text().toFloat();
            count++;
        }     
    }
    if(sum != 0)
    {
        avgMbL->setText(QString::number(sum / (float)count, 'f', 2));
        viewmbB->setText("View mb graph");
    }
}

void MainWindow::setupCoeffGraph(QCustomPlot *customPlot, QString name, QString colorName)
{
    customPlot->clearGraphs();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->addGraph(); // blue line
    if(colorName.startsWith("DarkBlue"))
        customPlot->graph(0)->setPen(QPen(Qt::darkBlue));
    else if(colorName.startsWith("DarkCyan"))
        customPlot->graph(0)->setPen(QPen(Qt::darkCyan));
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(30);
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(0.2);
    customPlot->yAxis->setRange(0, 1);
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setMargins(QMargins(0, 0, 0, 0));
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    customPlot->graph(0)->setName(name);
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->legend->removeItem(1);
    customPlot->replot();
}

void MainWindow::replot()
{
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.ccEnable[i] == 1)
        {
            plot[i]->xAxis->setRange(ccLastTime, xSizeArray[xSizeId], Qt::AlignRight);
            if(ySizeId == 1)
                plot[i]->graph(0)->rescaleValueAxis();
            else
                plot[i]->yAxis->setRange(0, 1);

            plot[i]->replot();
        }
    }

    ccPlot->xAxis->setRange(ccLastTime, xSizeArray[xSizeId], Qt::AlignRight);

    if(ySizeId == 1)
        ccPlot->graph(0)->rescaleValueAxis();
    else
        ccPlot->yAxis->setRange(0, 1);

    ccPlot->replot();
}

void MainWindow::savePick(PICK_INFO *pick)
{
    QString sta(pick->sta); QString chan(pick->chan); QString net(pick->net); QString loc(pick->loc);

    QString msecS = QString::number(pick->coeffTime, 'f', 3).section(".", 1, 1);
    QDateTime ct;
    ct.setTime_t((int)pick->coeffTime);
    ct = ct.addMSecs(msecS.toInt());
    ct = ct.addSecs(3600 * 9);

    QString rootDir = con.pickDir + "/" + ct.toString("yyyy") + "/" + ct.toString("yyyyMMddhhmmss.zzz") + "/"
            + sta + "_" + chan;

    QDir dir(rootDir);
    if(!dir.exists()) dir.mkpath(".");
    QFile file(rootDir + "/pickInfo");

    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &file );
        stream << ct.toString("yyyyMMddhhmmss.zzz") << " " << QString::number(pick->coeffTime, 'f', 3) << "\n";
        stream << sta << " " << chan << " " << net << " " << loc << " SPS:" << QString::number(pick->sps) << " Threshold:" << QString::number(pick->threshold, 'f', 2)
               << " nRefData:" << QString::number(pick->nRefData) << " pOffset:" << QString::number(pick->pOffset) << "\n";
        stream << "P_Arrival:" << QString::number(pick->pArrivalTime, 'f', 3) << " Max_Amp:" << QString::number(pick->maxAmp, 'f', 2)
               << " Per:" << QString::number(pick->per, 'f', 2) << " Max_AmpTime:" << QString::number(pick->maxAmpTime, 'f', 3)
               << " mb:" << QString::number(pick->mb, 'f', 2) << "\n";

        for(int i=0;i<pick->nRefData;i++)
        {
            QString refDataFileName(pick->refFileName[i]);
            stream << refDataFileName << ":" << QString::number(pick->eachCoeff[i], 'f', 6) << "\n";
        }
        file.close();
    }

    /*
wfdisc sta=KSA chan=HHZ time=1160357725.0028 nsamp=359999 samprate=100.000
chanid=-1 instype="-" jdate=2006282 lddate="2016-Nov-04"
calib=1.0000 calper=1.00
data
    */

    file.setFileName(rootDir + "/inputData.asc");

    ct = ct.addSecs( -(3600 * 9) );
    QDate t; t.setDate(ct.toString("yyyy").toInt(), ct.toString("MM").toInt(), ct.toString("dd").toInt());
    int jdate = t.dayOfYear(); QString jdateS;
    if(jdate < 10)
        jdateS = "00" + QString::number(jdate);
    else if(jdate < 100 && jdate >= 10)
        jdateS = "0" + QString::number(jdate);
    else if(jdate >= 100)
        jdateS = QString::number(jdate);

    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &file );
        stream << "wfdisc sta=" << sta << " chan=" << chan << " time=" << QString::number(pick->coeffTime, 'f', 3) << " nsamp=" << QString::number(pick->nSamp)
               << " samprate=" << QString::number(pick->sps) << "\n";
        stream << "chanid=-1 instype=\"-\" jdate=" << ct.toString("yyyy") << jdateS << " lddate="
               << QDateTime::currentDateTimeUtc().toString("yyyy-MMM-dd") << "\n";
        stream << "calib=1.0000 calper=1.00" << "\n" << "data" << "\n";

        for(int i=0;i<pick->nSamp;i++)
            stream << QString::number(pick->inpData[i], 'f', 3) << "\n";
        file.close();
    }
}

void MainWindow::detailBClicked()
{
    QObject *senderObj = sender();
    int i = senderObj->objectName().right(1).toInt();

    if(timeL[i]->text() != "")
    {
        QString timeS = timeL[i]->text();
        timeS = timeS.left(4) + "/" + timeS.left(4) + timeS.section("-",1, 1) + timeS.section("-",2, 2).section(" ", 0, 0)
                + timeS.section(" ", 1, 1).section(":", 0, 0) + timeS.section(" ", 1, 1).section(":", 1, 1)
                + timeS.section(" ", 1, 1).section(":", 2, 2);
        QString path = con.pickDir + "/" + timeS + "/" + staNameL[i]->text() + "_" + staNameL[i]->objectName();
        ViewPickReport *report = new ViewPickReport(path);
        report->show();

        connect(report, SIGNAL(finished()), report, SLOT(deleteLater()));
    }
}

void MainWindow::viewmbBClicked()
{
    QStringList staList, mbList;
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(mbL[i]->text() != "")
        {
            staList << staNameL[i]->text();
            mbList << mbL[i]->text();
        }
    }
    mbGraph *mbgraph = new mbGraph(staList, mbList, con.nkinfoFileName);
    mbgraph->show();

    connect(mbgraph, SIGNAL(finished()), mbgraph, SLOT(deleteLater()));
}

void MainWindow::actionExitClicked()
{
    if( !QMessageBox::question( this,
                "Exit",
                "KGXDetector will be close.",
                "Confirm",
                "Cancel",
                QString::null, 1, 1 ) )
    {
        close();
    }
}

void MainWindow::actionPrintClicked()
{
    /*
    QPrinter printer;
    QPainter painter;
    painter.begin(&printer);
    double xscale = printer.pageRect().width()/double(this->width());
    double yscale = printer.pageRect().height()/double(this->width());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                      printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);
    this->render(&painter);
    */
}

void MainWindow::actionTempClicked()
{
    CallLocation *calllocation = new CallLocation();
    QThread *tempThread = new QThread();
    calllocation->moveToThread(tempThread);

    // find nk5 using nk4
    con.locStanRef = 3;
    double t = 1473381029.968;

    // find nk4 using nk3
    //double t = 1452043829.678;
    //con.locStanRef = 2;

    // find nk3 using nk2
    //double t = 1360637900.258;
    //con.locStanRef = 1;

    // find nk2 using nk1
    //double t = 1243212911.910;
    //con.locStanRef = 0;

    calllocation->setup(con.pickDir, con.locStanRef, t, con.nkinfoFileName, con.gridlatB, con.gridlatE, con.gridlonB, con.gridlonE);

    tempThread->start();
    connect(tempThread, SIGNAL(started()), calllocation, SLOT(getLoc()));

    connect(calllocation, SIGNAL(finished()), calllocation, SLOT(deleteLater()));
    connect(calllocation, SIGNAL(destroyed()), tempThread, SLOT(quit()));
    connect(tempThread, SIGNAL(finished()), tempThread, SLOT(deleteLater()));
    connect(tempThread, SIGNAL(destroyed()), this, SLOT(actionPrintClicked()));
}

void MainWindow::hZoomOutBClicked()
{
    if(xSizeId != 3)
        xSizeId++;
    if(xSizeId == 3)
    {
        for(int i=0;i<MAX_SCNL;i++)
            if(con.ccEnable[i] == 1)
                plot[i]->xAxis->setTickStep(60);
        ccPlot->xAxis->setTickStep(60);
    }
    else
    {
        for(int i=0;i<MAX_SCNL;i++)
            if(con.ccEnable[i] == 1)
                plot[i]->xAxis->setTickStep(30);
        ccPlot->xAxis->setTickStep(30);
    }
    replot();
}

void MainWindow::hZoomInBClicked()
{ 
    if(xSizeId != 0)
        xSizeId--;
    if(xSizeId == 3)
    {
        for(int i=0;i<MAX_SCNL;i++)
            if(con.ccEnable[i] == 1)
                plot[i]->xAxis->setTickStep(60);
        ccPlot->xAxis->setTickStep(60);
    }
    else
    {
        for(int i=0;i<MAX_SCNL;i++)
            if(con.ccEnable[i] == 1)
                plot[i]->xAxis->setTickStep(30);
        ccPlot->xAxis->setTickStep(30);
    }
    replot();
}

void MainWindow::scaleBClicked()
{
    if(ySizeId == 1)
    {
        ui->scaleB->setText("Auto Scale");
        ySizeId = 0;
    }
    else
    {
        ui->scaleB->setText("Pixed Scale");
        ySizeId = 1;
    }
    replot();
}

void MainWindow::blinkBClicked()
{
    if(blinkTimer->isActive())
    {
        blinkTimer->stop();

        event.blink = 0;
        ui->ccBox->setStyleSheet("QGroupBox { background-color : white; }");
        ui->ccBox->setStyleSheet("QGroupBox { border: 2px solid gray; border-radius: 9px; margin-top: 0.5em; } "
                                 "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px; }");
    }
}

void MainWindow::actionResetClicked()
{
    if( !QMessageBox::question( this,
                "Reset All",
                "KGXDetector will be reset.",
                "Confirm",
                "Cancel",
                QString::null, 1, 1 ) )
    {
        deleteWidget();
        setup();
        blinkBClicked();
    }
}

void MainWindow::deleteWidget()
{
    for(int i=0;i<MAX_SCNL;i++)
    {
        if(con.ccEnable[i] == 1)
        {
            ui->graphLayout->removeWidget(plot[i]);
            plot[i]->deleteLater();
        }
    }
    ui->graphLayout->removeWidget(ccPlot);
    ccPlot->deleteLater();
    ui->graphLayout->removeWidget(line);
    line->deleteLater();

    for(int i=0;i<MAX_SCNL;i++)
    {
        ui->gridLayout->removeWidget(staNameL[i]);
        ui->gridLayout->removeWidget(timeL[i]);
        ui->gridLayout->removeWidget(coeffL[i]);
        ui->gridLayout->removeWidget(mbL[i]);
        ui->gridLayout->removeWidget(detailB[i]);
        staNameL[i]->deleteLater();
        timeL[i]->deleteLater();
        coeffL[i]->deleteLater();
        mbL[i]->deleteLater();
        detailB[i]->deleteLater();
    }

    myLayout->removeWidget(legend1); myLayout->removeWidget(legend2); myLayout->removeWidget(legend3);
    myLayout->deleteLater(); legend1->deleteLater(); legend2->deleteLater(); legend3->deleteLater();
    avgLabel->deleteLater(); avgCoeffL->deleteLater(); avgMbL->deleteLater(); viewmbB->deleteLater();
}

void MainWindow::actionEventListClicked()
{
    QDir rootdir(con.pickDir);
    QStringList maindirs;
    QString filterName = "20*";
    maindirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    if(maindirs.count() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("No EventList.");
        msgBox.exec();
        return;
    }
    else
    {
        EventList *eventlist = new EventList(con.pickDir, con.stanRef, con.nkinfoFileName, con.locStanRef);
        eventlist->show();

        connect(eventlist, SIGNAL(finished()), eventlist, SLOT(deleteLater()));
    }
}

void MainWindow::actionConfigureClicked()
{
    config->setup(con);
    config->show();
}

void MainWindow::recvParamFromConfig(CONFIGURE config)
{
    deleteWidget();
    memcpy(&con, &config, sizeof(CONFIGURE));
    setup();
}

void MainWindow::writeLogCoeff(COEFF *coeff)
{
    QDate today = QDate::currentDate();
    QDir d = con.pickDir + "/log";
    if(!d.exists()) d.mkpath(".");
    QFile file(con.pickDir + "/log/correlogramDetector_" + today.toString("yyyyMMdd") + ".txt");

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream( &file );
        QDateTime now = QDateTime::currentDateTimeUtc();
        stream << now.toString("[yyyy-MM-dd hh:mm:ss]") << " " << QString(coeff->sta) << " "
               << QString(coeff->chan) << " StartTime:" << QString::number(coeff->startTime, 'f', 3) << " "
               << "Nsamp:" << QString::number(coeff->nSamp) << "\n";
        file.close();
    }

    // detele log file before 5 days
    today = today.addDays(-5);
    file.setFileName(con.pickDir + "/log/correlogramDetector_" + today.toString("yyyyMMdd") + ".txt");
    if(file.exists())
        file.remove();
}

void MainWindow::writeLogPick(PICK_INFO *pick)
{
    QDate today = QDate::currentDate();
    QDir d = con.pickDir + "/log";
    if(!d.exists()) d.mkpath(".");
    QFile file(con.pickDir + "/log/xcorrDetector_" + today.toString("yyyyMMdd") + ".txt");

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream( &file );
        QDateTime now = QDateTime::currentDateTimeUtc();
        stream << now.toString("[yyyy-MM-dd hh:mm:ss]") << " " << QString(pick->sta) << " "
               << QString(pick->chan) << " CoeffTime:" << QString::number(pick->coeffTime, 'f', 3) << " "
               << "MaxCoeff.:" << QString::number(pick->maxCoeff, 'f', 6) << " "
               << "mb:" << QString::number(pick->mb, 'f', 2) << "\n";
        file.close();
    }

    // detele log file before 5 days
    today = today.addDays(-5);
    file.setFileName(con.pickDir + "/log/xcorrDetector_" + today.toString("yyyyMMdd") + ".txt");
    if(file.exists())
        file.remove();
}
