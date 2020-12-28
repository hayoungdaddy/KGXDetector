/**
 * @file                config.h
 * @author              YunYeoWoong (KIGAM)
 * @date                12 Sep 2016
 * @brief               Config class
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>

#include "common.h"

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

    CONFIGURE config;
    void setup(CONFIGURE);

    QLineEdit *staLE[MAX_SCNL];
    QLineEdit *chanLE[MAX_SCNL];
    QLineEdit *thresLE[MAX_SCNL];
    QLineEdit *shiftimeLE[MAX_SCNL];
    QCheckBox *ccEnableCB[MAX_SCNL];
    QComboBox *ccChanCB;
    QLineEdit *ccThresLE;

public slots:
    void saveConfig();
    void afilebrobclicked1();
    void afilebrobclicked2();
    void afilebrobclicked3();
    void datadirbclicked();

private:
    Ui::Config *ui;

private slots:
    void testScript1();

signals:
    void sendParamToMainWindow(CONFIGURE);
};

#endif // CONFIG_H
