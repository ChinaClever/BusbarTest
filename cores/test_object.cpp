/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_object.h"

Test_Object::Test_Object(QObject *parent) : QThread(parent)
{
    isRun = false;
    mPacket = sDataPacket::bulid();
    mItem = Cfg::bulid()->item;
    mPro = mPacket->getPro();
    mDev = mPacket->getDev();
    mSour = mPacket->getDev(0);
    mDt = &(mDev->devType);
    mBusData = get_share_mem();
    QTimer::singleShot(500,this,SLOT(initFunSlot()));
}

Test_Object::~Test_Object()
{
    isRun = false;
    wait();
}
