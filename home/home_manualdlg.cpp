/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_manualdlg.h"
#include "ui_home_manualdlg.h"

Home_ManualDlg::Home_ManualDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_ManualDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
}

Home_ManualDlg::~Home_ManualDlg()
{
    delete ui;
}

bool Home_ManualDlg::inputCheck()
{
    bool ret = ui->shellRadio->isChecked();
    if(ret) ret = ui->screenRadio->isChecked();
    if(ret) ret = ui->beepRradio->isChecked();
    if(ret) ret = ui->ledRadio->isChecked();
    if(ret) ret = ui->keyRadio->isChecked();
    if(ret) ret = ui->curRadio->isChecked();
    if(ret) ret = ui->verRadio->isChecked();

    return ret;
}

void Home_ManualDlg::on_okBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        this->accept();
    } else {
        this->close();
    }
}
