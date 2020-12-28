#include "showallpicks.h"
#include "ui_showallpicks.h"

/**
 * @file                showallpicks.cpp
 */

ShowAllPicks::ShowAllPicks(QString rootDir, QVector<QString> pList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowAllPicks)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    rDir = rootDir;

    ui->pickTable->setColumnWidth(0, 180);

    ui->pickTable->setRowCount(0);

    for(int i=1;i<pList.count();i++)
    {
        ui->pickTable->setRowCount(ui->pickTable->rowCount()+1);

        QDateTime et = QDateTime::fromString(pList.at(i).section(".", 0, 0),"yyyyMMddhhmmss");
        ui->pickTable->setItem(i-1, 0, new QTableWidgetItem(et.toString("yyyy-MM-dd hh:mm:ss.") + pList.at(i).section(".", 1, 1)));
        QDir d(rDir + "/" + et.toString("yyyy") + "/" + et.toString("yyyyMMddhhmmss.") + pList.at(i).section(".", 1, 1));
        QStringList dirs;
        QString filterName = "*";
        dirs = d.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);
        ui->pickTable->setItem(i-1, 1, new QTableWidgetItem(dirs[0]));

        QFile file(d.path() + "/" + dirs[0] + "/pickInfo");

        if(file.exists())
        {
            if( file.open( QIODevice::ReadOnly ) )
            {
                QTextStream stream(&file);
                QString line;
                line = stream.readLine(); line = stream.readLine();

                // threshold
                QString thresholdStr = line.section(" ",5,5).section(":",1,1);

                // max amp, p
                line = stream.readLine();
                QString pTime = line.section(" ", 0, 0).section(":", 1, 1);
                QString maxAmp = line.section(" ", 1, 1).section(":", 1, 1);
                QString per = line.section(" ", 2, 2).section(":", 1, 1);
                QString aTime = line.section(" ", 3, 3).section(":", 1, 1);
                QString mb = line.section(" ", 4, 4).section(":", 1, 1);

                // each coeff.
                QStringList thres;
                int refCount = 0;
                while(!stream.atEnd())
                {
                    line = stream.readLine();
                    thres << line.section(":",1,1).left(4);
                    refCount++;
                };

                ui->pickTable->setItem(i-1, 2, new QTableWidgetItem(thresholdStr));

                for(int j=3;j<3+refCount;j++)
                {
                    if(thres[j-3].toDouble() >= 0)
                        ui->pickTable->setItem(i-1, j, new QTableWidgetItem(thres[j-3]));
                    else
                        ui->pickTable->setItem(i-1, j, new QTableWidgetItem("--"));
                    if(thres[j-3].toFloat() >= thresholdStr.toFloat())
                        ui->pickTable->item(i-1, j)->setBackground(Qt::red);
                }

                QDateTime t;
                t.setTime_t( (uint)pTime.toDouble() );
                ui->pickTable->setItem(i-1, 9, new QTableWidgetItem(t.toString("HH:mm:ss") + "." + pTime.section(".", 1, 1)));
                ui->pickTable->setItem(i-1, 10, new QTableWidgetItem(maxAmp));
                ui->pickTable->setItem(i-1, 11, new QTableWidgetItem(per));
                t.setTime_t( (uint)aTime.toDouble() );
                ui->pickTable->setItem(i-1, 12, new QTableWidgetItem(t.toString("HH:mm:ss") + "." + aTime.section(".", 1, 1)));
                ui->pickTable->setItem(i-1, 13, new QTableWidgetItem(mb));
                ui->pickTable->setItem(i-1, 14, new QTableWidgetItem("View"));

                file.close();
            }
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

    connect(ui->pickTable, SIGNAL(cellClicked(int,int)), this, SLOT(clickPickTable(int,int)));
}

ShowAllPicks::~ShowAllPicks()
{
    delete ui;
}

void ShowAllPicks::clickPickTable(int row, int col)
{
    if(col == 13)
    {
        QString temp = ui->pickTable->item(row, 0)->text();
        QString sta = ui->pickTable->item(row, 1)->text();
        QString msec = temp.section(".", 1, 1);
        temp = temp.section(".", 0, 0);
        QDateTime dt = QDateTime::fromString(temp, "yyyy-MM-dd hh:mm:ss");
        temp = dt.toString("yyyyMMddhhmmss") + "." + msec;
        ViewPickReport *report = new ViewPickReport(rDir + "/" + dt.toString("yyyy") + "/" + temp + "/" + sta);
        report->show();

        connect(report, SIGNAL(finished()), report, SLOT(deleteLater()));
    }
}
