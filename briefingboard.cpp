#include "briefingboard.h"
#include "ui_briefingboard.h"

/**
 * @file                briefingboard.cpp
 */

BriefingBoard::BriefingBoard(QString rootDir, QString firstPick, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BriefingBoard)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    rDir = rootDir; // CONFIGURE.pickDir

    et = QDateTime::fromString(firstPick.section(".", 0, 0), "yyyyMMddhhmmss");

    QDir rootdir(rDir + "/" + firstPick.left(4));
    QStringList dirs;
    QString filterName = "20*";
    dirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    for(int i=0;i<dirs.count();i++) // all dirs in firstPicks year
    {
        QDateTime pt = QDateTime::fromString(dirs[i].section(".", 0, 0), "yyyyMMddhhmmss");
        if(pt.toTime_t() - et.toTime_t() <= MAX_TIME_GAP)
        {
            QDir subdir(rDir + "/" + firstPick.left(4) + "/" + dirs[i]);
            QStringList subdirs;
            QString filterName = "*";
            subdirs = subdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

            for(int j=0;j<subdirs.count();j++)
            {
                if(subdirs[j].startsWith("ccArray"))
                {
                    haveCCpickList.push_back(subdirs[0]);               // staName_chanName
                    haveCCpickList.push_back(dirs[i]);                  // this dirs[i] have ccArray

                    if(subdirs[0].section("_", 1, 1).startsWith("HHZ"))
                        ddelta = myRound((1.0 / 100), 2);
                    else
                        ddelta = myRound((1.0 / 20), 2);

                    //break;
                }
                else if(subdirs[j].startsWith("location"))
                {
                    QFile file(rDir + "/" + dirs[i].left(4) + "/" + dirs[i] + "/" + subdirs[j] + "/result.txt");

                    if(file.open(QIODevice::ReadOnly))
                    {
                        QTextStream stream(&file);
                        QString line;

                        while(!stream.atEnd())
                        {
                            line = stream.readLine();
                            double lat, lon, reflat, reflon, dist, azim;
                            if(line.startsWith("Location"))
                            {
                                line = stream.readLine();
                                ui->latL->setText(line.section("/", 2, 2).section(":", 1, 1));
                                lat = line.section("/", 2, 2).section(":", 1, 1).toDouble();
                                ui->lonL->setText(line.section("/", 3, 3).section(":", 1, 1));
                                lon = line.section("/", 3, 3).section(":", 1, 1).toDouble();
                            }
                            else if(line.startsWith("Reference"))
                            {
                                line = stream.readLine();
                                ui->locLabel->setText("Location Info.(Ref.:" + line.section("/", 0, 0).section(":", 1, 1) + ")");
                                reflat = line.section("/", 1, 1).section(":", 1, 1).toDouble();
                                reflon = line.section("/", 2, 2).section(":", 1, 1).toDouble();
                                int rtn = geo_to_km(reflat, reflon, lat, lon, &dist, &azim);
                                ui->distL->setText(QString::number(dist, 'f', NUM_DECIMAL_FOR_DIST));
                                ui->azimL->setText(QString::number(azim, 'f', NUM_DECIMAL_FOR_AZIM));
                                break;
                            }
                        }
                        file.close();
                    }
                }
                else
                {
                    if(pickList.isEmpty())
                    {
                        QVector<QString> pName;
                        pickList.push_back(pName);
                        pickList[0].push_back(subdirs[j]);      // staName_chanName
                        pickList[0].push_back(dirs[i]);         // dir Name

                        continue;
                    }
                    int newSta = 1;
                    for(int k=0;k<pickList.count();k++)
                    {
                        if(pickList[k].at(0).startsWith(subdirs[j]))
                        {
                            newSta = 0;
                            pickList[k].push_back(dirs[i]);
                            break;
                        }
                    }
                    if(newSta == 1)
                    {
                        QVector<QString> pName;
                        pickList.push_back(pName);
                        pickList[pickList.count()-1].push_back(subdirs[j]);      // staName_chanName
                        pickList[pickList.count()-1].push_back(dirs[i]);         // dir Name
                    }
                }
            }
        }
    }

/*
            int NccEnable = subdirs.count();
            if(NccEnable >= 2)
            {
                for(int j=0;j<subdirs.count();j++)
                {
                    if(subdirs[j].startsWith("ccArray"))
                    {
                        haveCCpickList.push_back(subdirs[0]);               // staName_chanName
                        haveCCpickList.push_back(dirs[i]);                  // this dirs[i] have ccArray

                        if(subdirs[0].section("_", 1, 1).startsWith("HHZ"))
                            ddelta = myRound((1.0 / 100), 2);
                        else
                            ddelta = myRound((1.0 / 20), 2);

                        break;
                    }
                    if(subdirs[j].startsWith("location"))
                    {
                        QFile file(rDir + "/" + dirs[i].left(4) + "/" + dirs[i] + "/" + subdirs[j] + "/result.txt");

                        if(file.open(QIODevice::ReadOnly))
                        {
                            QTextStream stream(&file);
                            QString line;

                            while(!stream.atEnd())
                            {
                                line = stream.readLine();
                                double lat, lon, reflat, reflon, dist, azim;
                                if(line.startsWith("Location"))
                                {
                                    line = stream.readLine();
                                    ui->latL->setText(line.section("/", 2, 2).section(":", 1, 1));
                                    lat = line.section("/", 2, 2).section(":", 1, 1).toDouble();
                                    ui->lonL->setText(line.section("/", 3, 3).section(":", 1, 1));
                                    lon = line.section("/", 3, 3).section(":", 1, 1).toDouble();
                                }
                                else if(line.startsWith("Reference"))
                                {
                                    line = stream.readLine();
                                    ui->locLabel->setText("Location Info.(Ref.:" + line.section("/", 0, 0).section(":", 1, 1) + ")");
                                    reflat = line.section("/", 1, 1).section(":", 1, 1).toDouble();
                                    reflon = line.section("/", 2, 2).section(":", 1, 1).toDouble();
                                    int rtn = geo_to_km(reflat, reflon, lat, lon, &dist, &azim);
                                    ui->distL->setText(QString::number(dist, 'f', NUM_DECIMAL_FOR_DIST));
                                    ui->azimL->setText(QString::number(azim, 'f', NUM_DECIMAL_FOR_AZIM));
                                    break;
                                }
                            }
                            file.close();
                        }
                    }
                }
            }

            if(pickList.isEmpty())
            {
                QVector<QString> pName;
                pickList.push_back(pName);
                pickList[0].push_back(subdirs[0]);      // staName_chanName
                pickList[0].push_back(dirs[i]);         // dir Name
                continue;
            }

            int newSta = 1;
            for(int j=0;j<pickList.count();j++)
            {
                if(pickList[j].indexOf(subdirs[0]) != -1)
                {
                    pickList[j].push_back(dirs[i]);
                    newSta = 0;
                    break;
                }
            }

            if(newSta == 1)
            {
                QVector<QString> pName;
                pickList.push_back(pName);
                pickList[pickList.count()-1].push_back(subdirs[0]);
                pickList[pickList.count()-1].push_back(dirs[i]);
            }
        }
    }
*/

    setup();

    makeInfo(0);

    ccplot = new QCustomPlot(this);
    ui->cclayout->addWidget(ccplot);

    line = new QFrame(this);
    line->setGeometry(QRect(/* ... */));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: rgb(71, 107, 41);");
    line->setPalette(Qt::black);
    ui->cclayout->addWidget(line);

    int NccEnable = haveCCpickList.count() / 2;

    for(int i=0;i<NccEnable;i++)
    {
        plot[i] = new QCustomPlot(this);
        ui->cclayout->addWidget(plot[i]);
        setupCoeffGraph(plot[i], haveCCpickList.at(i*2)); // pickList
        drawGraph(plot[i], haveCCpickList.at(i*2), haveCCpickList.at(i*2+1));
    }

    int start, end;
    start = ccNum.indexOf(NccEnable);
    end = ccNum.lastIndexOf(NccEnable);

    QVector<double> time, data, threshold;
    for(int i=start;i<=end;i++)
    {
        if(NccEnable != 0)
        {
            time.push_back(myRound(ccTime.at(i), 2));
            data.push_back(ccData.at(i) / NccEnable);
            threshold.push_back(ccThreshold.at(i));
        }
    }

    double max = 0;
    int maxId;
    for(int i=0;i<data.count();i++)
    {
        if(data.at(i) > max)
        {
            max = data.at(i);
            maxId = data.indexOf(max);
        }
    }

    if(NccEnable != 0)
    {
    ccplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    QCPGraph *graph = ccplot->addGraph();
    QCPGraph *treGraph = ccplot->addGraph();
    graph->setName("Cross Correlograms");
    graph->setPen(QPen(Qt::darkBlue));
    treGraph->setPen(QPen(Qt::red));
    ccplot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ccplot->xAxis->setDateTimeFormat("hh:mm:ss");
    ccplot->xAxis->setAutoTickStep(false);
    ccplot->xAxis->setTickStep(10);
    ccplot->yAxis->setAutoTickStep(false);
    ccplot->yAxis->setTickStep(0.2);
    ccplot->legend->setVisible(true);
    ccplot->legend->setFont(QFont("Helvetica", 9));
    ccplot->legend->setMargins(QMargins(0, 0, 0, 0));
    ccplot->legend->removeItem(1);
    ccplot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    graph->setData(time, data);
    treGraph->setData(time, threshold);
    ccplot->yAxis->setRange(0, 1);
    ccplot->xAxis->setRange(time.at(0), 60, Qt::AlignLeft);

    QCPItemTracer *phaseTracer = new QCPItemTracer(ccplot);
    QCPItemTracer *itemDemoPhaseTracer;
    ccplot->addItem(phaseTracer);
    itemDemoPhaseTracer = phaseTracer;
    phaseTracer->setGraph(graph);
    phaseTracer->setGraphKey(myRound(time.at(maxId), 2));
    phaseTracer->setInterpolating(true);
    phaseTracer->setStyle(QCPItemTracer::tsCircle);
    phaseTracer->setPen(QPen(Qt::red));
    phaseTracer->setBrush(Qt::red);
    phaseTracer->setSize(7);

    QCPItemText *phaseTracerText = new QCPItemText(ccplot);
    ccplot->addItem(phaseTracerText);
    phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignTop);
    phaseTracerText->position->setCoords(1.0, 0.01);
    QDateTime et;
    et.setTime_t( time.at(maxId) );
    phaseTracerText->setText("Max Coefficient:"+QString::number(max,'f',NUM_DECIMAL_FOR_COEFF)+"\nTime:"+et.toString("yyyy-MM-dd hh:mm:ss.")+
                             QString::number(myRound(time.at(maxId), 2) - ddelta, 'f', 2).section(".", 1, 1));
    phaseTracerText->setTextAlignment(Qt::AlignLeft);
    phaseTracerText->setFont(QFont(font().family(), 9));
    phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

    QCPItemCurve *phaseTracerArrow = new QCPItemCurve(ccplot);
    ccplot->addItem(phaseTracerArrow);
    phaseTracerArrow->start->setParentAnchor(phaseTracerText->left);
    phaseTracerArrow->startDir->setParentAnchor(phaseTracerArrow->start);
    phaseTracerArrow->startDir->setCoords(-40, 0);
    phaseTracerArrow->end->setParentAnchor(phaseTracer->position);
    phaseTracerArrow->end->setCoords(10, 10);
    phaseTracerArrow->endDir->setParentAnchor(phaseTracerArrow->end);
    phaseTracerArrow->endDir->setCoords(30, 30);
    phaseTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
    phaseTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (phaseTracerText->bottom->pixelPoint().y()-phaseTracerText->top->pixelPoint().y())*0.85));
    }

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(makeInfo(int)));
}

BriefingBoard::~BriefingBoard()
{
    delete ui;
}

void BriefingBoard::makeInfo(int n)     // n is standard reference, 0 is max
{
    for(int i=0;i<pickList.count();i++)
    {
        QString time, mb;
        double max = 0;
        for(int j=1;j<pickList[i].count();j++)
        {
            QFile file(rDir + "/" + pickList[i].at(j).left(4) + "/" + pickList[i].at(j) + "/" + pickList[i].at(0) + "/pickInfo");

            if( file.open( QIODevice::ReadOnly ) )
            {
                QTextStream stream(&file);
                QString line;
                line = stream.readLine(); line = stream.readLine();

                // threshold
                if(j == 1)
                {
                    pickList[i].push_back(line.section(" ",5,5).section(":",1,1));
                    int id = staThrValue.indexOf(pickList[i].at(0).section("_", 0, 0));
                    if( id == -1)
                    {
                        staThrValue.push_back(pickList[i].at(0).section("_", 0, 0));
                        staThrValue.push_back(line.section(" ",5,5).section(":",1,1));
                    }
                }

                // max amp, p
                line = stream.readLine();
                QString tempmb = line.section(" ", 4, 4).section(":", 1, 1);

                // each coeff.
                if(n == 0)
                {
                    while(!stream.atEnd())
                    {
                        line = stream.readLine();
                        if(max < line.section(":",1,1).toDouble())
                        {
                            max = line.section(":",1,1).toDouble();
                            mb = tempmb;
                            time = pickList[i].at(j);
                        }
                    }
                }
                else
                {
                    for(int k=0;k<=n-1;k++)
                        line = stream.readLine();
                    if(max < line.section(":",1,1).toDouble())
                    {
                        max = line.section(":",1,1).toDouble();
                        mb = tempmb;
                        time = pickList[i].at(j);
                    }
                }
                file.close();
            }
        }

        QDateTime maxT = QDateTime::fromString(time.section(".", 0, 0), "yyyyMMddhhmmss");
        timeL[i]->setText( maxT.toString("yyyy-MM-dd hh:mm:ss.") + time.section(".", 1, 1) );
        if(max != 0) { coeffL[i]->setText(QString::number(max, 'f', NUM_DECIMAL_FOR_COEFF)); detailB[i]->setText("View"); }
        else { coeffL[i]->setText(""); detailB[i]->setText(""); }
        mbL[i]->setText(mb);
    }

    getAvg();
}

void BriefingBoard::drawGraph(QCustomPlot *plot, QString staName, QString pickName)
{
    QVector<double> time, data, threshold;

    QFile file(rDir + "/" + pickName.left(4) + "/" + pickName + "/ccArray/" + staName.section("_", 0, 0));

    double tres;

    int xid = staThrValue.indexOf(staName.section("_", 0, 0));
    tres = staThrValue.at(xid+1).toDouble();
    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream stream(&file);
        QString line, timeS;
        int id;
        timeS = stream.readLine();
        time.push_back(myRound(timeS.toDouble(), 2));
        /* c */ double delta = ddelta;
        line = stream.readLine();
        data.push_back(line.toDouble());
        threshold.push_back(tres);

        int count = 1;
        while(!stream.atEnd())
        {
            line = stream.readLine();
            if(line == "nan" || line == "0.000000" || line.toDouble() > 1.0)
                break;

            data.push_back(line.toDouble());
            time.push_back(myRound(time.at(0) + (delta * count), 2));
            threshold.push_back(tres);

            id = ccTime.indexOf(myRound(time.last(), 2));
            if(id == -1 && ccTime.empty())
            {
                ccTime.push_back(myRound(time.last(), 2));
                ccData.push_back(data.last());
                ccThreshold.push_back(threshold.last());
                ccNum.push_back(1);
            }
            else if(id == -1 && ccTime.last() < time.last())
            {
                ccTime.push_back(myRound(time.last(), 2));
                ccData.push_back(data.last());
                ccThreshold.push_back(threshold.last());
                ccNum.push_back(1);
            }
            else if(id != -1)
            {
                ccData.replace(id, ccData.at(id) + data.last());
                ccNum.replace(id, ccNum.at(id) + 1);
            };

            count++;
        }

        file.close();
    }

    plot->graph(0)->setData(time, data);
    plot->graph(1)->setData(time, threshold);
    plot->yAxis->setRange(0, 1);
    plot->xAxis->setRange(time.at(0), 60, Qt::AlignLeft);
}

void BriefingBoard::getAvg()
{
    int count = 0;
    float sum = 0;
    for(int i=0;i<pickList.count();i++)
    {
        if(coeffL[i]->text().toFloat() != 0 || timeL[i]->text().toDouble() != 0)
        {
            sum += coeffL[i]->text().toFloat();
            count++;
        }
    }
    if(sum != 0) avgCoeffL->setText(QString::number(sum / (float)count, 'f', NUM_DECIMAL_FOR_COEFF));

    count = 0; sum = 0;
    for(int i=0;i<pickList.count();i++)
    {
        if(mbL[i]->text() != "")
        {
            sum += mbL[i]->text().toFloat();
            count++;
        }
        if(sum != 0) avgMbL->setText(QString::number(sum / (float)count, 'f', 2));
    }
}

void BriefingBoard::setup()
{
    QFont f1( "Sans", 11, QFont::Normal);

    for(int i=0;i<5;i++)
        label[i] = new QLabel(this);
    label[0]->setText("Station");
    label[1]->setText("Time(KST)");
    label[2]->setText("Coefficient");
    label[3]->setText("mb(ARS)");
    label[4]->setText("Detail View");

    int i;

    for(i=0;i<5;i++)
    {
        label[i]->setFont(f1);
        label[i]->setAlignment(Qt::AlignCenter);
        ui->gridLayout->addWidget(label[i], 0, i);
    }

    for(i=0;i<pickList.count();i++)
    {
        staNameL[i] = new QLabel(this);
        staNameL[i]->setText(pickList[i].at(0).section("_", 0, 0));
        staNameL[i]->setObjectName(pickList[i].at(0).section("_", 1, 1));
        staNameL[i]->setFrameStyle(1);
        staNameL[i]->setAlignment(Qt::AlignCenter);
        staNameL[i]->setFont(f1);
        timeL[i] = new QLabel(this);
        timeL[i]->setFrameStyle(1);
        timeL[i]->setAlignment(Qt::AlignCenter);
        timeL[i]->setStyleSheet("padding: 1px 8px 1px 8px;");
        timeL[i]->setFont(f1);
        coeffL[i] = new QLabel(this);
        coeffL[i]->setFrameStyle(1);
        coeffL[i]->setAlignment(Qt::AlignCenter);
        coeffL[i]->setFont(f1);
        mbL[i] = new QLabel(this);
        mbL[i]->setFrameStyle(1);
        mbL[i]->setAlignment(Qt::AlignCenter);
        mbL[i]->setFont(f1);

        detailB[i] = new QPushButton(this);
        detailB[i]->setFont(f1);
        detailB[i]->setText("View");
        detailB[i]->setObjectName("detailB" + QString::number(i));
        detailB[i]->setStyleSheet("border-style:solid;border-color: black;border-width:1px;");

        ui->gridLayout->addWidget(staNameL[i], i+1, 0);
        ui->gridLayout->addWidget(timeL[i], i+1, 1);
        ui->gridLayout->addWidget(coeffL[i], i+1, 2);
        ui->gridLayout->addWidget(mbL[i], i+1, 3);
        ui->gridLayout->addWidget(detailB[i], i+1, 4);

        connect(detailB[i], SIGNAL(clicked(bool)), this, SLOT(detailBClicked()));
    }

    junk1 = new QLabel(this); ui->gridLayout->addWidget(junk1, i+1, 0);
    junk2 = new QLabel(this); junk2->setText("Average"); junk2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->gridLayout->addWidget(junk2, i+1, 1);
    avgCoeffL = new QLabel(this); avgCoeffL->setFrameStyle(1); avgCoeffL->setAlignment(Qt::AlignCenter); avgCoeffL->setFont(f1);
    ui->gridLayout->addWidget(avgCoeffL, i+1, 2);
    avgMbL = new QLabel(this); avgMbL->setFrameStyle(1); avgMbL->setAlignment(Qt::AlignCenter); avgMbL->setFont(f1);
    ui->gridLayout->addWidget(avgMbL, i+1, 3);
    junk3 = new QPushButton(this); ui->gridLayout->addWidget(junk3, i+1, 4);
    junk3->setEnabled(false); junk3->setFlat(true);
}

void BriefingBoard::detailBClicked()
{
    QObject *senderObj = sender();
    int i = senderObj->objectName().right(1).toInt();

    if(timeL[i]->text() != "")
    {
        QString timeS = timeL[i]->text();
        timeS = timeS.left(4) + "/" + timeS.left(4) + timeS.section("-",1, 1) + timeS.section("-",2, 2).section(" ", 0, 0)
                + timeS.section(" ", 1, 1).section(":", 0, 0) + timeS.section(" ", 1, 1).section(":", 1, 1)
                + timeS.section(" ", 1, 1).section(":", 2, 2);
        QString path = rDir + "/" + timeS + "/" + staNameL[i]->text() + "_" + staNameL[i]->objectName();
        ViewPickReport *report = new ViewPickReport(path);
        report->show();

        connect(report, SIGNAL(finished()), report, SLOT(deleteLater()));
    }
}

void BriefingBoard::setupCoeffGraph(QCustomPlot *customPlot, QString name)
{
    customPlot->clearGraphs();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(Qt::darkCyan));
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(10);
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(0.2);
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
