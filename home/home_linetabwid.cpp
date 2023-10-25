/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_linetabwid.h"

Home_LineTabWid::Home_LineTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mDev = sDataPacket::bulid()->getDev();
    mItem = Cfg::bulid()->item;
    mData = &(mDev->line);
}

void Home_LineTabWid::initWid()
{
    QString title = tr("相数据列表");
    QStringList header;
    header << tr("开关") << tr("电流") << tr("电压") << tr("有功功率")  << tr("功率因数") << tr("电能");
    initTableWid(header, 3, title);
    mBusData = get_share_mem();
}


void Home_LineTabWid::appendItem(sObjectData *dev)
{
    for(int i=0; i<dev->lineNum; ++i) {
        QStringList listStr;

        if( mItem->modeId == START_BUSBAR){
            if(1 == dev->sw[i]) listStr << tr("开"); else  listStr << tr("关");
        }
        else{
            if(2 == dev->sw[i]) listStr << tr("开"); else if(1 == dev->sw[i])  listStr << tr("关");
        }
        listStr << QString::number(dev->cur.value[i]/COM_RATE_CUR,'f',2)+"A";
        listStr << QString::number(dev->vol.value[i]/COM_RATE_VOL,'f',1)+"V";

        listStr << QString::number(dev->pow.value[i]/COM_RATE_POW,'f',3)+"kW";
        listStr << QString::number(dev->pf[i]/COM_RATE_PF,'f',2);
        listStr << QString::number(dev->ele[i]/COM_RATE_ELE,'f',2)+"kWh";
        setTableRow(i, listStr);

        setItemColor(i, 1, dev->cur.status[i]);
        setItemColor(i, 2, dev->vol.status[i]);
        setItemColor(i, 3, dev->pow.status[i]);
    }
}

void Home_LineTabWid::timeoutDone()
{
    clearTable();
    if(mBusData->box[mItem->addr-1].loopNum) {
        appendItem(&(mBusData->box[mItem->addr-1].data));
    }
}
