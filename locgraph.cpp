#include "locgraph.h"
#include "ui_locgraph.h"

#include "qcustomplot.h"

LocGraph::LocGraph(QString rootDir, QString firstPick, QString nkInfoFile, int locStanRef, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocGraph)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QString locDirName = "";

    LocStanRef = locStanRef;

    QDateTime et = QDateTime::fromString(firstPick.section(".", 0, 0), "yyyyMMddhhmmss");

    QDir rootdir(rootDir + "/" + firstPick.left(4));
    QStringList dirs;
    QString filterName = "20*";
    dirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    for(int i=0;i<dirs.count();i++) // all dirs in firstPicks year
    {
        QDateTime pt = QDateTime::fromString(dirs[i].section(".", 0, 0), "yyyyMMddhhmmss");
        if(pt.toTime_t() - et.toTime_t() <= MAX_TIME_GAP)
        {
            QDir subdir(rootDir + "/" + firstPick.left(4) + "/" + dirs[i]);
            QStringList subdirs;
            QString filterName = "*";
            subdirs = subdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

            int NccEnable = subdirs.count();
            if(NccEnable >= 2)
            {
                for(int j=0;j<subdirs.count();j++)
                {
                    if(subdirs[j].startsWith("location"))
                    {
                        locDirName = rootDir + "/" + firstPick.left(4) + "/" + dirs[i] + "/location";
                        break;
                    }
                }
            }
            if(locDirName != "")
                break;
        }
    }

    QFile file(locDirName + "/result.txt");
    if(!file.exists())
        return;

    readLocFile(locDirName);
    readNKFile(nkInfoFile);
    setGraph(ui->customplot);
}

LocGraph::~LocGraph()
{
    delete ui;
}

void LocGraph::readLocFile(QString locDirName)
{
    QFile file(locDirName + "/result.txt");

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;

        while(!stream.atEnd())
        {
            line = stream.readLine();
            if(line.startsWith("Location"))
            {
                line = stream.readLine();
                ui->locTable->setRowCount(ui->locTable->rowCount()+1);
                //ui->locTable->setItem(0, 0, new QTableWidgetItem("This Event"));
                ui->locTable->setItem(0, 1, new QTableWidgetItem(line.section("/", 2, 2).section(":", 1, 1)));
                ui->locTable->setItem(0, 2, new QTableWidgetItem(line.section("/", 3, 3).section(":", 1, 1)));
            }
            else if(line.startsWith("Grid"))
            {
                line = stream.readLine();
                ui->gridLatB->setText(line.section("-", 0, 0).section(":", 1, 1));
                ui->gridLatE->setText(line.section("-", 1, 1));
                line = stream.readLine();
                ui->gridLonB->setText(line.section("-", 0, 0).section(":", 1, 1));
                ui->gridLonE->setText(line.section("-", 1, 1));
            }
            else if(line.startsWith("Reference"))
            {
                line = stream.readLine();
                ui->refName->setText(line.section("/", 0, 0).section(":", 1, 1));
                LocStanRef = line.section("/", 0, 0).section(":", 1, 1).right(1).toInt() - 1;
                ui->refLat->setText(line.section("/", 1, 1).section(":", 1, 1));
                ui->refLon->setText(line.section("/", 2, 2).section(":", 1, 1));
                QString temp = line.section("/", 3, 3).section(":", 1, 1);
                QString temp2 = temp.section(".", 1, 1);
                temp = temp.section(".", 0, 0);
                QDateTime ot; ot.setTime_t( temp.toDouble() );
                ui->refOT->setText(ot.toString("yyyy-MM-dd hh:mm:ss") + "." + temp2);

                int count = 0;
                while(1)
                {
                    line = stream.readLine();
                    if(line.startsWith("Observed"))
                        break;
                    ui->refTable->setRowCount(ui->refTable->rowCount()+1);
                    ui->refTable->setItem(count, 0, new QTableWidgetItem(line.section(":", 0, 0)));
                    QString temp = line.section(":", 1, 1);
                    QString temp2 = temp.section(".", 1, 1);
                    temp = temp.section(".", 0, 0);
                    QDateTime ot; ot.setTime_t( temp.toDouble() );
                    ui->refTable->setItem(count, 1, new QTableWidgetItem(ot.toString("yyyy-MM-dd hh:mm:ss") + "." + temp2));
                    count++;
                }

                count = 0;
                while(1)
                {
                    line = stream.readLine();
                    if(line.startsWith("Data"))
                        break;
                    ui->obTable->setRowCount(ui->obTable->rowCount()+1);
                    ui->obTable->setItem(count, 0, new QTableWidgetItem(line.section(":", 0, 0)));
                    QString temp = line.section(":", 1, 1);
                    QString temp2 = temp.section(".", 1, 1);
                    temp = temp.section(".", 0, 0);
                    QDateTime ot; ot.setTime_t( temp.toDouble() );
                    ot = ot.addSecs( 3600 * 9 );
                    ui->obTable->setItem(count, 1, new QTableWidgetItem(ot.toString("yyyy-MM-dd hh:mm:ss") + "." + temp2));
                    count++;
                }
            }
            else
            {
                y.push_back(line.section(" ", 0, 0).toDouble());
                x.push_back(line.section(" ", 1, 1).toDouble());
                z.push_back(line.section(" ", 2, 2).toDouble());
            }
        }
        file.close();
    }

    for(int i=0;i<ui->obTable->rowCount();i++)
    {
        ui->obTable->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->obTable->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
    for(int i=0;i<ui->refTable->rowCount();i++)
    {
        ui->refTable->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->refTable->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }

    //qDebug() << QString::number(x.count()) << " " << QString::number(y.count());
}

void LocGraph::readNKFile(QString nkInfoFile)
{
    QFile file(nkInfoFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;

        int count = 1;
        while(!stream.atEnd())
        {
            line = stream.readLine();
            if(line.startsWith("EventName"))
            {
                ui->locTable->setRowCount(ui->locTable->rowCount()+1);
                ui->locTable->setItem(count, 0, new QTableWidgetItem(line.section("=", 1, 1)));
            }
            else if(line.startsWith("Latitude"))
                ui->locTable->setItem(count, 1, new QTableWidgetItem(line.section("=", 1, 1)));
            else if(line.startsWith("Longitude"))
            {
                ui->locTable->setItem(count, 2, new QTableWidgetItem(line.section("=", 1, 1)));
                count++;
            }
        }
        file.close();
        newName = "NK" + QString::number(count);
        ui->locTable->setItem(0, 0, new QTableWidgetItem(newName));
    }

    for(int i=1;i<ui->locTable->rowCount();i++)
    {
        double dist = 0., azim = 0.;
        int rtn = geo_to_km(ui->locTable->item(0, 1)->text().toDouble(),
                            ui->locTable->item(0, 2)->text().toDouble(),
                            ui->locTable->item(i, 1)->text().toDouble(),
                            ui->locTable->item(i, 2)->text().toDouble(), &dist, &azim);
        ui->locTable->setItem(i, 3, new QTableWidgetItem(QString::number(dist, 'f', NUM_DECIMAL_FOR_DIST) + "Km"));
        ui->locTable->setItem(i, 4, new QTableWidgetItem(QString::number(azim, 'f', NUM_DECIMAL_FOR_AZIM) + "°"));
    }

    ui->locTable->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    ui->locTable->item(0, 1)->setTextAlignment(Qt::AlignCenter);
    ui->locTable->item(0, 2)->setTextAlignment(Qt::AlignCenter);
    ui->locTable->item(0, 0)->setForeground(Qt::red);
    ui->locTable->item(0, 1)->setForeground(Qt::red);
    ui->locTable->item(0, 2)->setForeground(Qt::red);

    for(int i=1;i<ui->locTable->rowCount();i++)
    {
        ui->locTable->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->locTable->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui->locTable->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui->locTable->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ui->locTable->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        if(i == LocStanRef + 1)
        {
            ui->locTable->item(i, 0)->setForeground(Qt::blue);
            ui->locTable->item(i, 1)->setForeground(Qt::blue);
            ui->locTable->item(i, 2)->setForeground(Qt::blue);
            ui->locTable->item(i, 3)->setForeground(Qt::blue);
            ui->locTable->item(i, 4)->setForeground(Qt::blue);
        }
    }
}

void LocGraph::setGraph(QCustomPlot *customPlot)
{
    int count = 0;

    // configure axis rect:
    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("Longitude");
    customPlot->yAxis->setLabel("Latitude");

    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(GRID_SPACING * 50);
    customPlot->xAxis->setNumberFormat("f");
    customPlot->xAxis->setNumberPrecision(3);

    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(GRID_SPACING * 50);
    customPlot->yAxis->setNumberFormat("f");
    customPlot->yAxis->setNumberPrecision(3);

    customPlot->xAxis->setRange(x.at(0), x.at(x.count()-1));
    customPlot->yAxis->setRange(y.at(0), y.at(y.count()-1));

    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(colorMap);
    double xnum, ynum;
    xnum = (x.at(x.count()-1) - x.at(0)) / GRID_SPACING + 1;
    ynum = (y.at(y.count()-1) - y.at(0)) / GRID_SPACING + 1;

    //qDebug() << QString::number(xnum);

    int nx = (int) xnum;
    int ny = (int) ynum;

    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(x.at(0), x.at(x.count()-1)), QCPRange(y.at(0), y.at(y.count()-1)));

    for(int yy=0;yy<ny;yy++)
    {
        for(int xx=0;xx<nx;xx++)
        {
            colorMap->data()->setCell(xx, yy, z.at(count));
            count++;
        }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    //colorScale->setDataRange(QCPRange(min, max));
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    //colorScale->axis()->setLabel("L-1 norm");

    // set the color gradient of the color map to one of the presets:
    //colorMap->setGradient(QCPColorGradient::gpPolar);
    colorMap->setGradient(QCPColorGradient::gpGeography);

    //colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();

    QVector<double> xxx, yyy;
    QVector<QString> name;
    for(int i=0;i<ui->locTable->rowCount();i++)
    {
        name.push_back(ui->locTable->item(i, 0)->text());
        yyy.push_back(ui->locTable->item(i, 1)->text().toDouble());
        xxx.push_back(ui->locTable->item(i, 2)->text().toDouble());
    }

    QCPGraph *graph = customPlot->addGraph();
    graph->setData(xxx, yyy);
    graph->setPen(QPen(Qt::black, 5));
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QPen(Qt::black), QBrush(Qt::white), 10));
    customPlot->replot();

    /*
    QCPGraph *graph2 = customPlot->addGraph();
    graph2->setName("Ref. Event");
    graph2->addData(xxx.at(LocStanRef+1), yyy.at(LocStanRef+1));
    graph2->setPen(QPen(Qt::black, 5));
    graph2->setLineStyle(QCPGraph::lsNone);
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QPen(Qt::black), QBrush(Qt::blue), 10));
    customPlot->replot();
    */

    QCPGraph *graph3 = customPlot->addGraph();
    graph3->addData(xxx.at(0), yyy.at(0));
    //graph3->setName("This Event");
    graph3->setName(newName);
    graph3->setPen(QPen(Qt::black, 5));
    graph3->setLineStyle(QCPGraph::lsNone);
    graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QPen(Qt::black), QBrush(Qt::red), 10));
    customPlot->replot();

    /*
    QCPGraph *graph4 = customPlot->addGraph();
    graph4->addData(xxx.at(0), yyy.at(0));
    graph4->addData(xxx.at(LocStanRef+1), yyy.at(LocStanRef+1));
    graph4->setPen(QPen(Qt::yellow, 2));
    graph4->setLineStyle(QCPGraph::lsLine);
    customPlot->replot();
    */

    customPlot->legend->removeItem(0);
    customPlot->legend->removeItem(0);
    customPlot->legend->removeItem(2);
    //customPlot->legend->setRowSpacing(-3);
    //customPlot->legend->setVisible(true);
    customPlot->legend->setVisible(false);
    customPlot->legend->setFont(QFont("Sans",12));

    double midX, midY;
    midX = (xxx.at(0) + xxx.at(LocStanRef+1)) / 2;
    midY = (yyy.at(0) + yyy.at(LocStanRef+1)) / 2;

    double distToThisWithRef, azimToThisWithRef;

    int rtn = geo_to_km(yyy.at(LocStanRef+1), xxx.at(LocStanRef+1), yyy.at(0), xxx.at(0), &distToThisWithRef, &azimToThisWithRef);

    /*
    if(azimToThisWithRef >= 180)
        azimToThisWithRef = azimToThisWithRef - 180;
    else
        azimToThisWithRef = azimToThisWithRef + 180;
        */

    /*
    if(azimToThisWithRef <= 90)
        azimToThisWithRef = -(90 - azimToThisWithRef);
    else
        azimToThisWithRef = 90 + azimToThisWithRef;
        */

    //qDebug() << QString::number(azimToThisWithRef, 'f', 4);
    //textLabel->setRotation(azimToThisWithRef);

    /*
    QCPItemText *textLabel = new QCPItemText(customPlot);
    customPlot->addItem(textLabel);
    textLabel->setPositionAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptPlotCoords);
    textLabel->position->setCoords(midX, midY+(GRID_SPACING * 10));
    textLabel->setText(QString::number(distToThisWithRef,'f', NUM_DECIMAL_FOR_DIST) + "km\n" + QString::number(azimToThisWithRef,'f', NUM_DECIMAL_FOR_AZIM) + "°");
    textLabel->setFont(QFont(font().family(), 11)); // make font a bit larger
    */

    for(int i=0;i<name.count();i++)
    {
        QCPItemText *textLabel = new QCPItemText(customPlot);
        customPlot->addItem(textLabel);
        textLabel->setPositionAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        textLabel->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel->position->setCoords(xxx.at(i), yyy.at(i)-(GRID_SPACING * 10));
        textLabel->setText(name.at(i).section(" ", 0, 0));
        textLabel->setFont(QFont(font().family(), 14)); // make font a bit larger
        if(i==0)
            textLabel->setColor(Qt::red);
        //if(i==LocStanRef+1)
        //   textLabel->setColor(Qt::blue);
        //textLabel->setPen(QPen(Qt::black)); // show black border around text
    }

    customPlot->xAxis->setRange(xxx.at(LocStanRef+1), GRID_SPACING * 300, Qt::AlignCenter);
    customPlot->yAxis->setRange(yyy.at(LocStanRef+1), GRID_SPACING * 300, Qt::AlignCenter);
}

