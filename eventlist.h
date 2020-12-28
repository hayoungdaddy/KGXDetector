/**
 * @file                eventlist.h
 * @brief               EventList class
 */

#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QDialog>

#include "common.h"
#include "viewpickreport.h"
#include "briefingboard.h"
#include "mysnapshot.h"
#include "showallpicks.h"
#include "mbgraph.h"
#include "locgraph.h"

namespace Ui {
class EventList;
}

class EventList : public QDialog
{
    Q_OBJECT

public:
    explicit EventList(QString rootDir = 0, int stanRef = 0, QString nkInfoFileName = 0, int locStanRef = 0, QWidget *parent = 0);
    ~EventList();

    void closeEvent(QCloseEvent *event)
    {
        emit finished();
    };

private:
    Ui::EventList *ui;

    QDateTime et;
    QVector< QVector<QString> > eName;
    QString rDir;
    QString NKInfoFileName;

    int stanRef;
    int nowIndex;
    int LocStanRef;
    QVector<QString> staList;
    QVector<PICK_INFO_FOR_EVENTLIST> pList;

    void setEventTable();
    void setPickTable(int);
    //bool compareNames(const QString&, const QString &);

private slots:
    void clickEventTable(int, int);
    void clickPickTable(int, int);
    void changeStanRef(int);
    void showAllPicks();

signals:
    void finished();
};

#endif // EVENTLIST_H
