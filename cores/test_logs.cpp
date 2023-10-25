/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_logs.h"
extern QString user_land_name();

Test_Logs::Test_Logs(QObject *parent) : Test_Object(parent)
{
}


Test_Logs *Test_Logs::bulid(QObject *parent)
{
    static Test_Logs* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_Logs(parent);
    return sington;
}

bool Test_Logs::appendLogItem(const QString &str, bool pass)
{
    sStateItem it;

    it.dev = mDev->devType.dev_type.split("_").first();
    it.user = mItem->user;
    it.sn = mDev->devType.sn;
    if(pass) {
        it.result = tr("通过");
    } else {
        it.result = tr("失败");
    }
    it.memo = str;
    if(it.sn.size()) mLogItems << it;

    return pass;
}

void Test_Logs::saveLogs()
{
    bool ret = writeLog();
    if(ret) {
        mPacket->updatePro(tr("测试日志保存"));
        writeLogs();
    } else {
        mLogItems.clear();
    }
}

bool Test_Logs::writeLog()
{
    Db_Tran db;
    sLogItem it;

    it.dev = mItem->modeId == START_BUSBAR?tr("始端箱"):tr("插接箱");
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = mDev->devType.sn;

    mItem->cnt.all += 1;
    if(mPro->result != Test_Fail) {
        it.result = tr("通过");
        mItem->cnt.ok += 1;
        if(mItem->cnt.num > 0) {
            mItem->cnt.num -= 1;
            if(!mItem->cnt.num)  {
                mItem->user.clear();
                Cfg::bulid()->write("user", mItem->user, "User");
            }
        }
    } else {
        mItem->cnt.err += 1;
        it.result = tr("失败");
    }

    Cfg::bulid()->writeCnt();
//    if(it.sn.isEmpty()) return false;
    return DbLogs::bulid()->insertItem(it);
}


void Test_Logs::writeLogs()
{
    Db_Tran db;
    for(int i=0; i<mLogItems.size(); ++i) {
        DbStates::bulid()->insertItem(mLogItems.at(i));
    }
    mLogItems.clear();
}

bool Test_Logs::updatePro(const QString &str, bool pass, int sec)
{
    if(mPro->step < Test_End) {
        if(!pass) appendLogItem(str, pass);
        mPacket->updatePro(str, pass, sec);
    }

    return pass;
}
