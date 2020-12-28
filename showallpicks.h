/**
 * @file                showallpicks.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               ShowAllPicks class
 */

#ifndef SHOWALLPICKS_H
#define SHOWALLPICKS_H

#include <QDialog>

#include "viewpickreport.h"

namespace Ui {
class ShowAllPicks;
}

class ShowAllPicks : public QDialog
{
    Q_OBJECT

public:
    explicit ShowAllPicks(QString rootDir, QVector<QString> pList, QWidget *parent = 0);
    ~ShowAllPicks();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

private:
    Ui::ShowAllPicks *ui;

    QString rDir;

private slots:
    void clickPickTable(int, int);

signals:
    void finished();
};

#endif // SHOWALLPICKS_H
