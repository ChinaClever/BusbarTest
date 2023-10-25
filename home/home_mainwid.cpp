/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_mainwid.h"
#include "ui_home_mainwid.h"

Home_MainWid::Home_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);

    initWid();
}

Home_MainWid::~Home_MainWid()
{
    delete ui;
}

void Home_MainWid::initWid()
{
    mWorkWid = new Home_WorkWid(ui->workWid);
    connect(mWorkWid, SIGNAL(startSig()), this, SIGNAL(startSig()));

    mLineTabWid = new Home_LineTabWid(ui->tabWidget);
    ui->tabWidget->insertTab(0, mLineTabWid, tr("相监测数据列表"));

//    mThTabWid = new Home_ThresholdTabWid(ui->tabWidget);
//    ui->tabWidget->insertTab(1, mThTabWid, tr("报警阈值信息"));

    mThTabWid1 = new Home_ThresholdTabWid(0 ,ui->tabWidget);
    ui->tabWidget->insertTab(1 , mThTabWid1, tr("电流报警阈值"));

    mThTabWid2 = new Home_ThresholdTabWid(1 ,ui->tabWidget);
    ui->tabWidget->insertTab(2 , mThTabWid2, tr("电压报警阈值"));

    mThTabWid3 = new Home_ThresholdTabWid(2 ,ui->tabWidget);
    ui->tabWidget->insertTab(3 , mThTabWid3, tr("线电压报警阈值"));

    mThTabWid4 = new Home_ThresholdTabWid(3 ,ui->tabWidget);
    ui->tabWidget->insertTab(4 , mThTabWid4, tr("功率报警阈值"));

    mThTabWid5 = new Home_ThresholdTabWid(4 ,ui->tabWidget);
    ui->tabWidget->insertTab(5 , mThTabWid5, tr("其他报警阈值"));

    mEnvTabWid = new Home_EnvTabWid(ui->tabWidget);
    ui->tabWidget->insertTab(6, mEnvTabWid, tr("传感器环境状态"));
}
