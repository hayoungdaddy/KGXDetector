#include "mbgraph.h"
#include "ui_mbgraph.h"

mbGraph::mbGraph(QStringList staNameList, QStringList mbList, QString nkInfoFileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mbGraph)
{
    ui->setupUi(this);
    ui->wave->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    NKInfoFileName = nkInfoFileName;

    double max = 0, min = 10;
    QVector<QString> labels;
    labels << "YNB" << "DNH" << "KSA" << "NSN" << "YKB" << "CHNB" << "USA0B" << "KS31" << "BRD" << "YNG";

    int totalNK = 0;
    QVector<double> nk_x[10], nk_y[10];
    QStringList nk_sta[10], nk_mb[10];

    QFile file(NKInfoFileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;

        while(!stream.atEnd())
        {
            line = stream.readLine();
            if(line.startsWith("EventName"))
            {
                while(1)
                {
                    line = stream.readLine();
                    if(line.startsWith("#######"))
                    {
                        totalNK++;
                        break;
                    }
                    else if(line.startsWith("OriginTime") || line.startsWith("Latitude") || line.startsWith("Longitude"))
                        continue;
                    else
                    {
                        nk_sta[totalNK] << line.section("=", 0, 0);
                        nk_mb[totalNK] << line.section("=", 2, 2);
                    }
                }
            }
        }
        file.close();
    }

    //qDebug() << mbList;
    double mbSum = 0;
    for(int i=0;i<mbList.count();i++)
        mbSum = mbSum + mbList[i].toDouble();

    mbAvg = mbSum / mbList.count();

    //qDebug() << mbAvg;

    // get Yield
    double imsY, kigamY, ringdalY;

    // IMS : mb = 4.0 + log(Y)
    imsY = pow(10, mbAvg-4.0);

    // KIGAM : mb = 4.28 + 0.84 * log(Y)
    kigamY = pow(10, (mbAvg-4.28)/0.84);

    // Ringdal : mb = 4.45 + 0.75 * log(Y)
    ringdalY = pow(10, (mbAvg-4.45)/0.75);

    ui->imsY->setText(QString::number(imsY, 'f', 1));
    ui->kigamY->setText(QString::number(kigamY, 'f', 1));
    ui->ringdalY->setText(QString::number(ringdalY, 'f', 1));

    for(int i=0;i<labels.count();i++)
    {
        for(int j=0;j<totalNK;j++)
        {
            for(int k=0;k<nk_sta[j].count();k++)
            {
                if(labels.at(i).startsWith(nk_sta[j][k]))
                {
                    nk_x[j].push_back(i+1);
                    nk_y[j].push_back(nk_mb[j][k].toDouble());

                    if(nk_mb[j][k].toDouble() >= max)
                        max = nk_mb[j][k].toDouble();
                    if(nk_mb[j][k].toDouble() < min)
                        min = nk_mb[j][k].toDouble();
                    break;
                }
            }
        }
    }

    QVector<double> x, y;
    for(int i=0;i<labels.count();i++)
    {
        for(int j=0;j<staNameList.count();j++)
        {
            if(labels.at(i).startsWith(staNameList[j]))
            {
                x.push_back(i+1);
                y.push_back(mbList[j].toDouble());

                if(mbList[j].toDouble() >= max)
                    max = mbList[j].toDouble();
                if(mbList[j].toDouble() < min)
                    min = mbList[j].toDouble();
                break;
            }
        }
    }

    ui->wave->addGraph();
    ui->wave->graph(0)->setData(x, y);

    ui->wave->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->wave->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red), QBrush(Qt::white), 6));
    ui->wave->graph(0)->setPen(QPen(Qt::red, 3));
    //ui->wave->graph(0)->setName("This");
    ui->wave->graph(0)->setName("NK" + QString::number(totalNK+1));


    //qDebug() << QString::number(totalNK);

    int count = 1;
    for(int i=totalNK;i>0;i--)
    {
        ui->wave->addGraph();
        ui->wave->graph(count)->setLineStyle(QCPGraph::lsLine);
        ui->wave->graph(count)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
        ui->wave->graph(count)->setData(nk_x[i-1], nk_y[i-1]);
        ui->wave->graph(count)->setName("NK"+QString::number(i));
        count++;
    }

    ui->wave->graph(1)->setPen(QPen(Qt::blue));
    ui->wave->graph(2)->setPen(QPen(Qt::red));
    ui->wave->graph(3)->setPen(QPen(Qt::darkBlue));
    ui->wave->graph(4)->setPen(QPen(Qt::green));
    ui->wave->graph(5)->setPen(QPen(Qt::magenta));

    ui->wave->legend->setVisible(true);
    ui->wave->legend->setFont(QFont("Helvetica", 11));
    ui->wave->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);

    ui->wave->xAxis->setLabel("Station");
    ui->wave->yAxis->setLabel("mb");
    ui->wave->yAxis->setAutoTickStep(false);
    ui->wave->yAxis->setTickStep(0.5);
    ui->wave->yAxis->setRange(min -1 , max + 1);

    ui->wave->xAxis->setTickStep(1);
    ui->wave->xAxis->setSubTickCount(0);
    ui->wave->xAxis->setAutoTicks(false);
    ui->wave->xAxis->setAutoTickLabels(false);
    ui->wave->xAxis->setTickVector(QVector<double>() << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10);
    ui->wave->xAxis->setTickVectorLabels(labels);
    ui->wave->xAxis->setRange(0, 11);

    connect(ui->nk1, SIGNAL(clicked(bool)), this, SLOT(nk1Bclicked()));
    connect(ui->nk2, SIGNAL(clicked(bool)), this, SLOT(nk2Bclicked()));
    connect(ui->nk3, SIGNAL(clicked(bool)), this, SLOT(nk3Bclicked()));
    connect(ui->nk4, SIGNAL(clicked(bool)), this, SLOT(nk4Bclicked()));
    connect(ui->nk5, SIGNAL(clicked(bool)), this, SLOT(nk5Bclicked()));
    connect(ui->nk6, SIGNAL(clicked(bool)), this, SLOT(nk6Bclicked()));
    connect(ui->quibB, SIGNAL(clicked(bool)), this, SLOT(accept()));
}

mbGraph::~mbGraph()
{
    delete ui;
}

void mbGraph::nk1Bclicked()
{
    if(ui->nk1->text().startsWith("Hide"))
    {
        ui->wave->graph(6)->setVisible(false);
        ui->nk1->setText("Show NK1");
    }
    else
    {
        ui->wave->graph(6)->setVisible(true);
        ui->nk1->setText("Hide NK1");
    }
    ui->wave->replot();
}

void mbGraph::nk2Bclicked()
{
    if(ui->nk2->text().startsWith("Hide"))
    {
        ui->wave->graph(5)->setVisible(false);
        ui->nk2->setText("Show NK2");
    }
    else
    {
        ui->wave->graph(5)->setVisible(true);
        ui->nk2->setText("Hide NK2");
    }
    ui->wave->replot();
}

void mbGraph::nk3Bclicked()
{
    if(ui->nk3->text().startsWith("Hide"))
    {
        ui->wave->graph(4)->setVisible(false);
        ui->nk3->setText("Show NK3");
    }
    else
    {
        ui->wave->graph(4)->setVisible(true);
        ui->nk3->setText("Hide NK3");
    }
    ui->wave->replot();
}

void mbGraph::nk4Bclicked()
{
    if(ui->nk4->text().startsWith("Hide"))
    {
        ui->wave->graph(3)->setVisible(false);
        ui->nk4->setText("Show NK4");
    }
    else
    {
        ui->wave->graph(3)->setVisible(true);
        ui->nk4->setText("Hide NK4");
    }
    ui->wave->replot();
}

void mbGraph::nk5Bclicked()
{
    if(ui->nk5->text().startsWith("Hide"))
    {
        ui->wave->graph(2)->setVisible(false);
        ui->nk5->setText("Show NK5");
    }
    else
    {
        ui->wave->graph(2)->setVisible(true);
        ui->nk5->setText("Hide NK5");
    }
    ui->wave->replot();
}

void mbGraph::nk6Bclicked()
{
    if(ui->nk6->text().startsWith("Hide"))
    {
        ui->wave->graph(1)->setVisible(false);
        ui->nk6->setText("Show NK6");
    }
    else
    {
        ui->wave->graph(1)->setVisible(true);
        ui->nk6->setText("Hide NK6");
    }
    ui->wave->replot();
}
