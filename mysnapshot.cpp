#include "mysnapshot.h"
#include "ui_mysnapshot.h"

/**
 * @file                mysnapshot.cpp
 */

MySnapShot::MySnapShot(QString rootDir, QString firstpick, QString lastpick, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySnapShot)
{
    ui->setupUi(this);

    QDateTime et = QDateTime::fromString(firstpick.section(".", 0, 0), "yyyy-MM-dd hh:mm:ss");
    QDateTime end = QDateTime::fromString(lastpick.section(".", 0, 0), "yyyy-MM-dd hh:mm:ss");

    QDir rootdir(rootDir + "/" + firstpick.left(4));
    QStringList dirs;
    QString filterName = "20*";
    dirs = rootdir.entryList(QStringList(filterName), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    QStringList list;
    for(int i=0;i<dirs.count();i++)
    {
        QDateTime pt = QDateTime::fromString(dirs[i].section(".", 0, 0), "yyyyMMddhhmmss");
        if(pt.toTime_t() - et.toTime_t() <= MAX_TIME_GAP)
        {
            QDir subdir(rootDir + "/" + firstpick.left(4) + "/" + dirs[i]);
            QStringList subdirs;
            QString filterName = "*";
            subdirs = subdir.entryList(QStringList(filterName), QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

            for(int j=0;j<subdirs.count();j++)
            {
                list << rootDir + "/" + firstpick.left(4) + "/" + subdir.dirName() + "/" + subdirs[j];
            }
        }
    }

    QDateTime last; last.setTime_t(1400000000);
    QString lastDir;
    for(int i=0;i<list.count();i++)
    {
        QFileInfo fi(list[i]);
        if(fi.lastModified() >= last)
        {
            last = fi.lastModified();
            lastDir = list[i];
        }
    }

    QPixmap *pix = new QPixmap(lastDir);
    ui->label->setPixmap(*pix);
    ui->label->setScaledContents(true);
}

MySnapShot::~MySnapShot()
{
    delete ui;
}
