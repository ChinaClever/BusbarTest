/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_envtabwid.h"

Home_EnvTabWid::Home_EnvTabWid(QWidget *parent) : ComTableWid(parent)
{
    mItem = Cfg::bulid()->item;
    initWid();
//    sDevData *dev = sDataPacket::bulid()->getDev();
}

void Home_EnvTabWid::initWid()
{
    QString title = tr("传感器信息");
    QStringList header;
    header << tr("传感器") << tr("监测值") << tr("最小值") << tr("最大值");
    initTableWid(header, 2, title);
    mBusData = get_share_mem();
}


void Home_EnvTabWid::setDataUnit(int id, sDataValue &unit)
{
    QString suffix = "℃";
    QString str = tr("温度");

    for(int i=0; i<4; ++i) {
        QStringList listStr;

        listStr << str;
        listStr << QString::number(unit.value[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.min[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.max[i]/COM_RATE_TEM,'f',1)+suffix;
        setTableRow(id+i, listStr);

        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i])) {
            unit.alarm[i] = 2;
            setAlarmBackgroundColor(id+i);
        } else {
            unit.alarm[i] = 0;
            setNormalBackgroundColor(id+i);
            setItemColor(i, 1, unit.alarm[i]);
        }
    }
}


void Home_EnvTabWid::appendItem()
{
    setDataUnit(0, (mBusData->box[mItem->addr-1].env.tem));
//    setDataUnit(1, mEnv->hum);
}

void Home_EnvTabWid::timeoutDone()
{
    clearTable();
    if(mBusData->box[mItem->addr-1].loopNum) {
        appendItem();
    }
}
