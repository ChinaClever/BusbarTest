/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"

Td_MainWid::Td_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Td_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mStatusTabWid = new Td_StatusTabWid(ui->groupBox);
    connect(this, SIGNAL(startSig()), mStatusTabWid, SLOT(startSlot()));
}

Td_MainWid::~Td_MainWid()
{
    delete ui;
}
