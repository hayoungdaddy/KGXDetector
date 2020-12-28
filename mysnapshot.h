/**
 * @file                mysnapshot.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               MySnapShot class
 */

#ifndef MYSNAPSHOT_H
#define MYSNAPSHOT_H

#include <QDialog>
#include <QDateTime>
#include <QDir>

#include "common.h"

namespace Ui {
class MySnapShot;
}

class MySnapShot : public QDialog
{
    Q_OBJECT

public:
    explicit MySnapShot(QString rootDir = 0, QString firstpick = 0, QString lastpick = 0, QWidget *parent = 0);
    ~MySnapShot();

private:
    Ui::MySnapShot *ui;
};

#endif // MYSNAPSHOT_H
