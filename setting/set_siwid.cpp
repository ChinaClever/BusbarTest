/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "set_siwid.h"
#include "ui_set_siwid.h"

Set_SiWid::Set_SiWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Set_SiWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mUnitWid = new Set_LineUnitWid(ui->groupBox);
    QTimer::singleShot(13,this,SLOT(initFunSlot()));
}

Set_SiWid::~Set_SiWid()
{
    delete ui;
}


void Set_SiWid::initFunSlot()
{
    this->setEnabled(false);
    mItem = Cfg::bulid()->item;
    mUnitWid->init(&(mItem->si_cfg) , 1);
    initType();
}

void Set_SiWid::initType()
{
    sSiCfg *dt = &(mItem->si); //设备类型
    ui->baudBox->setCurrentIndex(dt->si_baud);
    ui->buzzerBox->setCurrentIndex(dt->si_buzzer);
    ui->filterBox->setCurrentIndex(dt->si_filter-2);
    ui->iOFBox->setCurrentIndex(dt->si_iOF);
}

void Set_SiWid::updateType()
{
    sSiCfg *dt = &(mItem->si);
    dt->si_baud = ui->baudBox->currentIndex();
    dt->si_buzzer = ui->buzzerBox->currentIndex();
    dt->si_filter = ui->filterBox->currentIndex()+2;
    dt->si_iOF = ui->iOFBox->currentIndex();

}

bool Set_SiWid::inputCheck()
{
    return true;
}

bool Set_SiWid::dataSave()
{
    bool ret = inputCheck();
    if(ret) {
        updateType();
        mUnitWid->updateData();
    }

    return ret;
}

void Set_SiWid::enabledSlot(bool en)
{
    if(mItem->modeId != INSERT_BUSBAR) return;

    this->setEnabled(en);
    if(!en) {
        en = dataSave();
        if(en) {
            Cfg::bulid()->writeCfgDev();
        } else {
            emit errSig();
        }
    }
}
