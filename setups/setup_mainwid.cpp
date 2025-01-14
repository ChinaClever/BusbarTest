/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "versiondlg.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(rand()%13,this,SLOT(initFunSlot()));
    mItem = Cfg::bulid()->item;
    initSerial();
}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}

void Setup_MainWid::initFunSlot()
{
    initPcNum();
    initLogCount();
    initErrData();
    initAddr();

    if(ui->addrEdit->text().isEmpty()) {
        ui->addrEdit->setText("192.168.1.15");
        mItem->Service = ui->addrEdit->text();
    }

    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);
    QDate buildDate = QLocale(QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    ui->label_date->setText(buildDate.toString("yyyy-MM-dd"));
    QTimer::singleShot(3*1000,this,SLOT(checkPcNumSlot()));
}

void Setup_MainWid::initSerial()
{
    mSer1Wid = new SerialStatusWid(ui->serWid_1);
    mItem->coms.ser1 = mSer1Wid->initSerialPort(tr("SER 接口"));

    mSer2Wid = new SerialStatusWid(ui->serWid_2);
    mItem->coms.ser2 = mSer2Wid->initSerialPort(tr("LINK 接口"));

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    mItem->coms.source = mSourceWid->initSerialPort(tr("SI-PDU 参考源"));
}


void Setup_MainWid::initLogCount()
{
    Cfg *con = Cfg::bulid();
    int value = con->read("log_count", 10, "Sys").toInt();

    sCfgItem *item = con->item;
    item->logCount = value * 10000;
    ui->logCountSpin->setValue(value);
}


void Setup_MainWid::writeLogCount()
{
    int arg1 = ui->logCountSpin->value();
    mItem->logCount = arg1;
    Cfg::bulid()->write("log_count", arg1, "Sys");
}

void Setup_MainWid::updateErrData()
{
    sErrRange *item = &(mItem->err);
    item->volErr = ui->volErrBox->value() * 10;
    item->curErr = ui->curErrBox->value() * 10;
    item->powErr = ui->powErrBox->value() * 10;

    Cfg::bulid()->writeErrData();
}

void Setup_MainWid::initErrData()
{
    sErrRange *item = &(mItem->err);
    ui->volErrBox->setValue(item->volErr / 10.0);
    ui->curErrBox->setValue(item->curErr / 10.0);
    ui->powErrBox->setValue(item->powErr / 10.0);
}

void Setup_MainWid::on_saveBtn_clicked()
{
    static int flg = 0;
    QString str = tr("修改");

    bool ret = usr_land_jur();
    if(!ret) {
        MsgBox::critical(this, tr("你无权进行此操作"));
        return;
    }

    if(flg++ %2) {
        ret = false;
        updateErrData();
    } else {
        str = tr("保存");
    }

    ui->saveBtn->setText(str);
    ui->volErrBox->setEnabled(ret);
    ui->curErrBox->setEnabled(ret);
    ui->powErrBox->setEnabled(ret);
}

void Setup_MainWid::initAddr()
{
    Cfg *con = Cfg::bulid();
    QString value = con->read("service_addr", "", "Sys").toString();

    mItem->Service = value;
    ui->addrEdit->setText(value);
}

void Setup_MainWid::writeAddr()
{
    QString arg1 = ui->addrEdit->text();
    mItem->Service = arg1;
    Cfg::bulid()->write("service_addr", arg1, "Sys");
}

void Setup_MainWid::initPcNum()
{
    Cfg *con = Cfg::bulid();
    int value = con->read("pc_num", 0, "Sys").toInt();

    mItem->pcNum = value;
    ui->pcNumSpin->setValue(value);
}

void Setup_MainWid::checkPcNum()
{
    int num = mItem->pcNum;
    if(num < 1) {
        if(!usr_land_jur())
            MsgBox::warning(this, tr("请联系研发部设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
        else
            MsgBox::warning(this, tr("请自行设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
    }
}

void Setup_MainWid::checkAddr()
{
    QString str = ui->addrEdit->text();
    if(str.isEmpty())
        MsgBox::warning(this, tr("请设置服务端IP!"));
}

void Setup_MainWid::checkPcNumSlot()
{
    checkPcNum(); checkAddr();
    QTimer::singleShot(20*1000,this,SLOT(checkPcNumSlot()));

}


void Setup_MainWid::writePcNum()
{
    int arg1 = ui->pcNumSpin->value();
    mItem->pcNum = arg1;
    Cfg::bulid()->write("pc_num", arg1, "Sys");
}

void Setup_MainWid::on_verBtn_clicked()
{
    VersionDlg dlg(this);
    dlg.exec();
}

void Setup_MainWid::on_pcBtn_clicked()
{
    static int flg = 0;
    QString str = tr("修改");

    bool ret = usr_land_jur();
    if(!ret) {
        MsgBox::critical(this, tr("你无权进行此操作"));
        return;
    }

    if(flg++ %2) {
        ret = false;
        writePcNum();
        writeLogCount();
        writeAddr();
    } else {
        str = tr("保存");
    }

    ui->pcBtn->setText(str);
    ui->pcNumSpin->setEnabled(ret);
    ui->logCountSpin->setEnabled(ret);
    ui->addrEdit->setEnabled(ret);

    if(!ret) {
        bool res = true;
        QString str1 = tr("该服务端IP异常");
        QString ip = ui->addrEdit->text();
        for(int k=0; k<2; ++k) {
            res = cm_pingNet(ip);
            if(res) break;
        }
        if(res) mItem->online = true;
        if(!res) MsgBox::information(this,str1);
    }
}

