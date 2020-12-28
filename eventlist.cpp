#include "eventlist.h"
#include "ui_eventlist.h"

/**
 * @file                eventlist.cpp
 */

bool lessThan( const QVector<QString> &s1, const QVector<QString> &s2 )
{
    return ( QDateTime::fromString(s1.at(0), "yyyy-MM-dd hh:mm:ss").toTime_t() >
             QDateTime::fromString(s2.at(0), "yyyy-MM-dd hh:mm:ss").toTime_t() );
}

EventList::EventList(QString rootDir, int standardReference, QString nkInfoFileName, int locStanRef, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    stanRef = standardReference;
    ui->comboBox->setCurrentIndex(stanRef);
    nowIndex = 0;
    NKInfoFileName = nkInfoFileName;
    LocStanRef = locStanRef;

    ui->pickTable->setColumnWidth(0, 190);
    ui->eventTable->setColumnWidth(0, 180);
    ui->eventTable->setColumnWidth(1, 120);

    rDir = rootDir;         // CONFIGURE.pickDir
    QDir rootdir(rDir);
    QStringList maindirs;
    QString filterName = "20*";
    maindirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    // maindirs = yyyy, subdirs = yyyyMMddhhmmss.zzz
    for(int i=maindirs.count()-1;i>=0;i--)
    {
        QDir subdir(rootDir + "/" + maindirs[i]);
        QStringList subdirs;
        subdirs = subdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

        for(int j=0;j<subdirs.count();j++)
        {
            if(eName.isEmpty())
            {
                et = QDateTime::fromString(subdirs[j].section(".", 0, 0),"yyyyMMddhhmmss");
                QVector<QString> pName;
                eName.push_back(pName);
                eName[0].push_back(et.toString("yyyy-MM-dd hh:mm:ss"));
                eName[0].push_back(subdirs[j]);
            }
            else
            {
                QDateTime newEt = QDateTime::fromString(subdirs[j].section(".", 0, 0),"yyyyMMddhhmmss");
                if(newEt.toTime_t() - et.toTime_t() > MAX_TIME_GAP)
                {
                    et = QDateTime::fromString(subdirs[j].section(".", 0, 0),"yyyyMMddhhmmss");
                    QVector<QString> pName;
                    eName.push_back(pName);
                    eName[eName.count()-1].push_back(et.toString("yyyy-MM-dd hh:mm:ss"));
                    eName[eName.count()-1].push_back(subdirs[j]);
                }
                else if(newEt.toTime_t() - et.toTime_t() <= MAX_TIME_GAP)
                {
                    eName[eName.count()-1].push_back(subdirs[j]);
                }
            }
        }

        qSort(eName.begin(), eName.end(), lessThan);
    }

    setEventTable();
    setPickTable(nowIndex);

    ui->eventTable->setCurrentCell(0, 0);

    connect(ui->eventTable, SIGNAL(cellClicked(int,int)), this, SLOT(clickEventTable(int,int)));
    connect(ui->pickTable, SIGNAL(cellClicked(int,int)), this, SLOT(clickPickTable(int,int)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeStanRef(int)));
    connect(ui->showAllButton, SIGNAL(clicked(bool)), this, SLOT(showAllPicks()));
}

EventList::~EventList()
{
    delete ui;
}

void EventList::clickEventTable(int row, int col)
{
    if(col == 0)
    {
        nowIndex = row;
        changeStanRef(stanRef);
    }
    else if(col == 1)
    {
        nowIndex = row;
        changeStanRef(stanRef);
        QString temp = eName[nowIndex].at(1);
        //MySnapShot *shot = new MySnapShot(rDir, ui->pickTable->item(0, 0)->text(), ui->pickTable->item(count-1, 0)->text(), this);
        //shot->show();

        BriefingBoard *board = new BriefingBoard(rDir, temp);
        board->show();

        connect(board, SIGNAL(finished()), board, SLOT(deleteLater()));
    }
    else if(col == 2)
    {
        QStringList staNameList, mbList;
        nowIndex = row;
        changeStanRef(stanRef);

        for(int i=0;i<ui->pickTable->rowCount();i++)
        {
            staNameList << ui->pickTable->item(i, 1)->text().section("_", 0, 0);
            mbList << ui->pickTable->item(i, 2)->text();
        }

        mbGraph *mbgraph = new mbGraph(staNameList, mbList, NKInfoFileName);
        mbgraph->show();

        connect(mbgraph, SIGNAL(finished()), mbgraph, SLOT(deleteLater()));
    }
    else if(col == 3)
    {
        nowIndex = row;
        changeStanRef(stanRef);
        QString temp = eName[nowIndex].at(1);

        LocGraph *locgraph = new LocGraph(rDir, temp, NKInfoFileName, LocStanRef);
        locgraph->show();

        connect(locgraph, SIGNAL(finished()), locgraph, SLOT(deleteLater()));
    }
}

void EventList::clickPickTable(int row, int col)
{
    if(col == 10)
    {
        ViewPickReport *report = new ViewPickReport(pList.at(row).dirName);
        report->show();

        connect(report, SIGNAL(finished()), report, SLOT(deleteLater()));
    }
}

void EventList::changeStanRef(int nowStanRef)
{
    stanRef = nowStanRef;
    staList.clear();
    pList.clear();
    setPickTable(nowIndex);
}

void EventList::showAllPicks()
{
    ShowAllPicks *showallpicks = new ShowAllPicks(rDir, eName[nowIndex]);
    showallpicks->show();

    connect(showallpicks, SIGNAL(finished()), showallpicks, SLOT(deleteLater()));
}

void EventList::setEventTable()
{
    for(int i=0;i<eName.count();i++)
    {
        ui->eventTable->setRowCount(ui->eventTable->rowCount()+1);
        ui->eventTable->setItem(i, 0, new QTableWidgetItem(eName[i].at(0)));
        ui->eventTable->setItem(i, 1, new QTableWidgetItem("View"));
        ui->eventTable->setItem(i, 2, new QTableWidgetItem("View"));
        ui->eventTable->setItem(i, 3, new QTableWidgetItem("View"));
        ui->eventTable->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->eventTable->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui->eventTable->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui->eventTable->item(i, 3)->setTextAlignment(Qt::AlignCenter);
    }
}

void EventList::setPickTable(int index)
{    
    for(int i=1;i<eName[index].count();i++)
    {
        QDateTime et2 = QDateTime::fromString(eName[index].at(i).section(".", 0, 0),"yyyyMMddhhmmss");
        QDir d(rDir + "/" + et2.toString("yyyy") + "/" + et2.toString("yyyyMMddhhmmss.") + eName[index].at(i).section(".", 1, 1));
        QStringList dirs;
        QString filterName = "*";
        dirs = d.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(int j=0;j<dirs.count();j++)
        {
            if(staList.indexOf(dirs[j]) == -1 && !dirs[j].startsWith("ccArray") && !dirs[j].startsWith("location"))
            {
                staList.push_back(dirs[j]);
                PICK_INFO_FOR_EVENTLIST thisP;
                QFile file(d.path() + "/" + dirs[j] + "/pickInfo");
                if(file.exists())
                {
                    if( file.open( QIODevice::ReadOnly ) )
                    {
                        QTextStream stream(&file);
                        QString line;
                        thisP.sta = dirs[j];
                        thisP.dirName = d.path() + "/" + dirs[j];
                        line = stream.readLine(); line = stream.readLine();
                        thisP.threshold = line.section(" ",5,5).section(":",1,1);
                        line = stream.readLine();
                        thisP.pTime = line.section(" ", 0, 0).section(":", 1, 1);
                        thisP.mb = line.section(" ", 4, 4).section(":", 1, 1);
                        int refCount = 1;
                        while(!stream.atEnd())
                        {
                            line = stream.readLine();
                            thisP.maxCoeff[refCount] = line.section(":",1,1).toDouble();
                            refCount++;
                        };

                        file.close();

                        thisP.maxCoeff[0] = 0;

                        for(int k=1;k<refCount;k++)
                        {                            
                            if(thisP.maxCoeff[k] > thisP.maxCoeff[0])
                                thisP.maxCoeff[0] = thisP.maxCoeff[k];
                        }
                    }
                }

                pList.push_back(thisP);
            }
            else if(staList.indexOf(dirs[j]) != -1)
            {
                int staId = staList.indexOf(dirs[j]);
                PICK_INFO_FOR_EVENTLIST thisP;
                QFile file(d.path() + "/" + dirs[j] + "/pickInfo");
                if(file.exists())
                {
                    if( file.open( QIODevice::ReadOnly ) )
                    {
                        QTextStream stream(&file);
                        QString line;
                        thisP.sta = dirs[j];
                        thisP.dirName = d.path() + "/" + dirs[j];
                        line = stream.readLine(); line = stream.readLine();
                        thisP.threshold = line.section(" ",5,5).section(":",1,1);
                        line = stream.readLine();
                        thisP.pTime = line.section(" ", 0, 0).section(":", 1, 1);
                        thisP.mb = line.section(" ", 4, 4).section(":", 1, 1);
                        int refCount = 1;
                        while(!stream.atEnd())
                        {
                            line = stream.readLine();
                            thisP.maxCoeff[refCount] = line.section(":",1,1).toDouble();
                            refCount++;
                        };

                        file.close();

                        thisP.maxCoeff[0] = 0;
                        for(int k=1;k<refCount;k++)
                        {
                            if(thisP.maxCoeff[k] > thisP.maxCoeff[0])
                                thisP.maxCoeff[0] = thisP.maxCoeff[k];
                        }
                    }
                }
                if(pList.at(staId).maxCoeff[stanRef] < thisP.maxCoeff[stanRef])
                {
                    pList.replace(staId, thisP);
                }
            }
        }
    }

    ui->pickTable->setRowCount(0);

    for(int i=0;i<staList.count();i++)
    {
        if(pList.at(i).maxCoeff[stanRef] > 0)
        {
            ui->pickTable->setRowCount(ui->pickTable->rowCount()+1);

            QDateTime t;
            t.setTime_t( (uint)pList.at(i).pTime.toDouble() );
            ui->pickTable->setItem(ui->pickTable->rowCount()-1, 0, new QTableWidgetItem(t.toString("yy-MM-dd HH:mm:ss") + "." + pList.at(i).pTime.section(".", 1, 1)));
            ui->pickTable->setItem(ui->pickTable->rowCount()-1, 1, new QTableWidgetItem(pList.at(i).sta));
            ui->pickTable->setItem(ui->pickTable->rowCount()-1, 2, new QTableWidgetItem(pList.at(i).mb));

            for(int j=1;j<NUM_NK+1;j++)
            {
                if(pList.at(i).maxCoeff[j] > 0)
                    ui->pickTable->setItem(ui->pickTable->rowCount()-1, 2+j, new QTableWidgetItem(QString::number(pList.at(i).maxCoeff[j], 'f', NUM_DECIMAL_FOR_COEFF)));
                else
                    ui->pickTable->setItem(ui->pickTable->rowCount()-1, 2+j, new QTableWidgetItem("--"));
                if(pList.at(i).maxCoeff[j] >= pList.at(i).threshold.toDouble())
                    ui->pickTable->item(ui->pickTable->rowCount()-1, 2+j)->setBackground(Qt::red);
            }

            ui->pickTable->setItem(ui->pickTable->rowCount()-1, 9, new QTableWidgetItem(pList.at(i).threshold));
            ui->pickTable->setItem(ui->pickTable->rowCount()-1, 10, new QTableWidgetItem("View"));
        }
    }

    for(int i=0;i<ui->pickTable->rowCount();i++)
    {
        for(int j=0;j<ui->pickTable->columnCount();j++)
        {
            if(ui->pickTable->item(i, j)->text() != "")
                ui->pickTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
