#include "config.h"
#include "ui_config.h"

/**
 * @file                config.cpp
 */

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QLabel *label = new QLabel[6];
    label[0].setText("Station");
    label[1].setText("Channel");
    label[2].setText("Threshold");
    label[3].setText("Time for Shifting");
    label[4].setText("Correlograms\nEnable");
    label[5].setText("Correlograms");

    for(int i=0;i<5;i++)
    {
        label[i].setAlignment(Qt::AlignCenter);
        ui->glayout->addWidget(&label[i], 0, i);
    }

    for(int i=0;i<MAX_SCNL;i++)
    {
        staLE[i] = new QLineEdit;
        chanLE[i] = new QLineEdit;
        thresLE[i] = new QLineEdit;
        shiftimeLE[i] = new QLineEdit;
        ccEnableCB[i] = new QCheckBox;
        ccEnableCB[i]->setStyleSheet("margin-left:50%;margin-right:50%;");
        ui->glayout->addWidget(staLE[i], i+1, 0);
        ui->glayout->addWidget(chanLE[i], i+1, 1);
        ui->glayout->addWidget(thresLE[i], i+1, 2);
        ui->glayout->addWidget(shiftimeLE[i], i+1, 3);
        ui->glayout->addWidget(ccEnableCB[i], i+1, 4);
    }
    label[5].setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->glayout->addWidget(&label[5], MAX_SCNL+2, 0);

    ccChanCB = new QComboBox;
    QStringList chanList;
    chanList << "HHZ" << "BHZ";
    ccChanCB->addItems(chanList);
    ui->glayout->addWidget(ccChanCB, MAX_SCNL+2, 1);
    ccThresLE = new QLineEdit;
    ui->glayout->addWidget(ccThresLE, MAX_SCNL+2, 2);

    connect(ui->fbro1, SIGNAL(clicked(bool)), this, SLOT(afilebrobclicked1()));
    connect(ui->fbro2, SIGNAL(clicked(bool)), this, SLOT(afilebrobclicked2()));
    connect(ui->fbro3, SIGNAL(clicked(bool)), this, SLOT(afilebrobclicked3()));
    connect(ui->dataDirB, SIGNAL(clicked(bool)), this, SLOT(datadirbclicked()));
    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->test1, SIGNAL(clicked(bool)), this, SLOT(testScript1()));
}

Config::~Config()
{
    delete ui;
}

void Config::afilebrobclicked1()
{
    QString alarmfile;
    alarmfile = QFileDialog::getOpenFileName(this, "Select a script file for alarm", "/home", "*.*");
    ui->alarmfile1->setText(alarmfile);
}

void Config::afilebrobclicked2()
{
    QString nkinfofile;
    nkinfofile = QFileDialog::getOpenFileName(this, "Select a NK Info. file", "/home", "*.*");
    ui->nkinfofile->setText(nkinfofile);
}

void Config::afilebrobclicked3()
{
    QString segFile;
    segFile = QFileDialog::getOpenFileName(this, "Select a script file for data segmentation", "/home", "*.*");
    ui->segmentLE->setText(segFile);
}

void Config::datadirbclicked()
{
    QString datadir;
    datadir = QFileDialog::getExistingDirectory(this, "Select a data directory", "/home", QFileDialog::ShowDirsOnly);
    ui->dataDir->setText(datadir);
}

void Config::testScript1()
{
    QString temp;
    temp = "sh " + ui->alarmfile1->text() + " &";
    system(temp.toLatin1().data());
}

void Config::setup(CONFIGURE configure)
{
    configure.NccEnable = 0;
    ui->dataDir->setText(configure.pickDir);
    ui->alarmfile1->setText(configure.alarmFileName);
    if(configure.alarmEnable == 1) ui->acheck1->setChecked(true); else ui->acheck1->setChecked(false);
    ui->segmentLE->setText(configure.segmentFile);
    ui->nkinfofile->setText(configure.nkinfoFileName);
    ui->locStanRefCB->setCurrentIndex(configure.locStanRef);
    ui->gridLatB->setText(QString::number(configure.gridlatB, 'f', 4));
    ui->gridLatE->setText(QString::number(configure.gridlatE, 'f', 4));
    ui->gridLonB->setText(QString::number(configure.gridlonB, 'f', 4));
    ui->gridLonE->setText(QString::number(configure.gridlonE, 'f', 4));

    ui->zmqServerIP->setText(configure.zmqServerIP);
    ui->zmqPortForPick->setText(QString::number(configure.zmqPortForPick));
    ui->zmqPortForCoeff->setText(QString::number(configure.zmqPortForCoeff));
    ccThresLE->setText(QString::number(configure.ccthreshold,'f',2));

    for(int i=0;i<MAX_SCNL;i++)
    {
        if(!configure.staName[i].startsWith(" "))
        {
            staLE[i]->setText(configure.staName[i]); chanLE[i]->setText(configure.chanName[i]);
            thresLE[i]->setText(QString::number(configure.threshold[i], 'f', 2));
            shiftimeLE[i]->setText(QString::number(configure.timeShift[i], 'f', 3));
        }
        if(configure.ccEnable[i] == 1)
        {
            ccEnableCB[i]->setChecked(true);
            configure.NccEnable++;
        }
        else
            ccEnableCB[i]->setChecked(false);
    }

    if(configure.ccSps == 100) ccChanCB->setCurrentIndex(0);
    else ccChanCB->setCurrentIndex(1);

    ui->xcorrStanRefCB->setCurrentIndex(configure.stanRef);
}

void Config::saveConfig()
{
    config.NccEnable = 0;
    config.pickDir = ui->dataDir->text();
    config.alarmFileName = ui->alarmfile1->text();
    config.nkinfoFileName = ui->nkinfofile->text();
    config.locStanRef = ui->locStanRefCB->currentIndex();
    config.gridlatB = ui->gridLatB->text().toDouble();
    config.gridlatE = ui->gridLatE->text().toDouble();
    config.gridlonB = ui->gridLonB->text().toDouble();
    config.gridlonE = ui->gridLonE->text().toDouble();
    //if(ui->acheck1->isChecked()) config.alarmEnable = 1; else config.alarmEnable = 0;
    config.alarmEnable = 1;
    config.segmentFile = ui->segmentLE->text();

    config.zmqServerIP = ui->zmqServerIP->text();
    config.zmqPortForPick = ui->zmqPortForPick->text().toInt();
    config.zmqPortForCoeff = ui->zmqPortForCoeff->text().toInt();
    config.ccthreshold = ccThresLE->text().toFloat();

    for(int i=0;i<MAX_SCNL;i++)
    {
        config.staName[i] = staLE[i]->text(); config.chanName[i] = chanLE[i]->text();
        config.threshold[i] = thresLE[i]->text().toFloat(); config.timeShift[i] = shiftimeLE[i]->text().toFloat();

        if(ccEnableCB[i]->isChecked())
        {
            config.ccEnable[i] = 1;
            config.NccEnable++;
        }
        else
            config.ccEnable[i] = 0;
    }

    config.stanRef = ui->xcorrStanRefCB->currentIndex();
    if(ccChanCB->currentIndex() == 0) config.ccSps = 100;
    else config.ccSps = 20;

    emit sendParamToMainWindow(config);
    accept();
}
