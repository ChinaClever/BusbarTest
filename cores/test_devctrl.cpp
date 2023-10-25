/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_devctrl.h"

Test_DevCtrl::Test_DevCtrl(QObject *parent) : Test_Object(parent)
{

}

Test_DevCtrl *Test_DevCtrl::bulid(QObject *parent)
{
    static Test_DevCtrl* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_DevCtrl(parent);
    return sington;
}

void Test_DevCtrl::initFunSlot()
{
    mRtu = nullptr;
    mSiRtu = Ctrl_SiRtu::bulid(this);
    mIpRtu = Ctrl_IpRtu::bulid(this);
}

bool Test_DevCtrl::setCurTh(int i)
{
    bool ret = initDev();
    if(ret) {
        ret = mRtu->setCurTh(i);
    }
    return ret;
}

bool Test_DevCtrl::setVolTh(int i)
{
    bool ret = initDev();
    if(ret) {
        ret = mRtu->setVolTh(i);
    }
    return ret;
}


bool Test_DevCtrl::eleClean()
{
    bool ret = initDev();
    mRtu->eleClean();
    return ret;
}

bool Test_DevCtrl::factorySet()
{
    bool ret = initDev();
    if(ret) {
        ret = mRtu->factorySet();
    }
    return ret;
}

bool Test_DevCtrl::initDev()
{
    bool ret = true;
    mRtu = mSiRtu;

    return ret;
}
