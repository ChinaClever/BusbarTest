/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "printer_bartender.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : Test_Object(parent)
{

}


void Test_CoreThread::initFunSlot()
{
    mLogs = Test_Logs::bulid(this);
    mErr = Test_ErrRange::bulid(this);
    mRead = Test_DevRead::bulid(this);
    mCtrl = Test_DevCtrl::bulid(this);
    mNetWork = Test_NetWork::bulid(this);
    connect(mNetWork , SIGNAL(sendMACSig(QString)) , this , SLOT(getMacSlot(QString)));
    Printer_BarTender::bulid(this);
}

void Test_CoreThread::getMacSlot(QString str)
{
    if( str.size() >= 17 )
        this->mMacStr = str.right(17);
}

bool Test_CoreThread::hubPort()
{
    QString str = tr("设备 SER 级联口");
    bool ret = mItem->coms.ser1->isOpened();
    if(ret) {
        ret = mRead->readHub();
        if(ret) str += tr("正常");
        else str += tr("错误");
    } else {
        ret = true;
        str += tr("跳过");
    }

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::initDev()
{
    mLogs->updatePro(tr("即将开始"));
    bool ret  = false;
    ret = mRead->readDevData();

    return ret;
}

bool Test_CoreThread::volErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<5; ++k) {
        ret = mErr->volErr(i);
        if(ret) break; else mRead->readDevData();
    }

    QString str = tr("电压 L%1，期望电压=%2V，实测电压=%3V").arg(i+1)
            .arg(mSour->line.vol.value[i]).arg(mBusData->box[mItem->addr - 1].data.vol.value[i]/(COM_RATE_VOL));
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::curErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<5; ++k) {
        ret = mErr->curErr(i);
        if(ret) break; else mRead->readDevData();
    }
    QString str = tr("电流 L%1，期望电流=%2A，实测电流=%3A").arg(i+1)
            .arg(mSour->line.cur.value[i]/COM_RATE_CUR).arg(mBusData->box[mItem->addr - 1].data.cur.value[i]/COM_RATE_CUR);
    if(ret) str += tr("正常");
    else {
        if(mDev->line.cur.value[i]) {
            str += tr("错误");
        } else {
            str = tr("电流 L%1，错误，请接上负载，实测电流=0A").arg(i+1);
        }
    }

    return mLogs->updatePro(str, ret);
}


bool Test_CoreThread::eleErrRange(int i)
{
    bool ret = mItem->eleCheck;
    QString str = tr("电能 L%1，实测电能=%2Kwh").arg(i+1).arg(mBusData->box[mItem->addr-1].data.ele[i]/COM_RATE_ELE);
    if(ret) {
        if(1 != mBusData->box[mItem->addr-1].data.ele[i]) {
            str += tr("错误");
            ret = false;
        } else {
            str += tr("正常");
            ret = true;
        }
    } else {
        str += tr("跳过");
        ret = true;
    }

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::eleErrRange2(int i)
{
    bool ret = mItem->eleCheck;
    QString str = tr("电能 L%1，实测电能=%2Kwh").arg(i+1).arg(mBusData->box[mItem->addr-1].data.ele[i]/COM_RATE_ELE);
    if(ret) {
        if(2 != mBusData->box[mItem->addr-1].data.ele[i]) {
            str += tr("错误");
            ret = false;
        } else {
            str += tr("正常");
            ret = true;
        }
    } else {
        str += tr("跳过");
        ret = true;
    }

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::eleErrRange0(int i)
{
    QString str = tr("电能 L%1，实测电能=%2Kwh").arg(i+1).arg(mBusData->box[mItem->addr-1].data.ele[i]/COM_RATE_ELE);
    bool ret = false;
    if(0 != mBusData->box[mItem->addr-1].data.ele[i]) {
        str += tr("错误");
        ret = false;
    } else {
        str += tr("正常");
        ret = true;
    }

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::powErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<6; ++k) {
        ret = mErr->powErr(i);
        if(ret) break; else mRead->readDevData();
    }

    QString str = tr("功率 L%1，期望功率=%2kW，实测功率=%3kW").arg(i+1)
            .arg(mSour->line.pow[i]/COM_RATE_POW).arg(mDev->line.pow[i]/COM_RATE_POW);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::envErrRange()
{
    bool ret = true;
    for(int i = 0 ; i < 4 ; i++){
        ret = mErr->temErr(i);
        QString str = tr("传感器温度%1，温度%1=%2").arg(i+1).arg(mBusData->box[mItem->addr - 1].env.tem.value[0]);
        if(ret) str += tr("正常");
        else {
            if(mBusData->box[mItem->addr - 1].env.tem.value[0]) {
                str += tr("错误");
            } else {
                str = tr("请插入传感器，实测温度=0");
            }
        }

        ret = mLogs->updatePro(str, ret);
    }
    return ret;
}

bool Test_CoreThread::checkErrRange()
{
    int i = 0;
    bool res = true, ret = true;
    for(; i<mBusData->box[mItem->addr-1].loopNum; ++i) {
        ret = volErrRange(i); if(!ret) res = false;
        ret = curErrRange(i); if(!ret) res = false;
        ret = eleErrRange(i); if(!ret) res = false;
        if(ret){ret = powErrRange(i); if(!ret) res = false;}
    }
    if(res) res = envErrRange();

    return res;
}

bool Test_CoreThread::volAlarmErr(int i)
{
    QString str = tr("电压报警阈值 L%1 ").arg(i+1);
    bool ret = mErr->volAlarm(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::curAlarmErr(int i)
{
    QString str = tr("电流报警阈值 L%1 ").arg(i+1);
    bool ret = mErr->curAlarm(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::powAlarmErr(int i)
{
    QString str = tr("功率报警阈值 L%1 ").arg(i+1);
    bool ret = mErr->powAlarm(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::envAlarmErr()
{
    bool res = true;
    for(int i = 0 ; i < 4 ; i++){
        QString str = tr("温度%1报警阈值 ").arg(i+1);
        bool ret = mErr->temAlarm(i);
        if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
        mLogs->updatePro(str, ret);
    }

    return res;
}

bool Test_CoreThread::hzAlarmErr()
{
    bool res = true;
    QString str = tr("频率报警阈值 ");
    bool ret = mErr->hzAlarm();
    if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
    mLogs->updatePro(str, ret);
    return res;
}

bool Test_CoreThread::totalPowAlarmErr()
{
    bool res = true;
    QString str = tr("总功率报警阈值 ");
    bool ret = mErr->totalPowAlarm();
    if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
    mLogs->updatePro(str, ret);
    return res;
}

bool Test_CoreThread::zeroLineCurAlarmErr()
{
    bool res = true;
    QString str = tr("零线电流报警阈值 ");
    bool ret = mErr->zeroLineCurAlarm();
    if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
    mLogs->updatePro(str, ret);
    return res;
}

bool Test_CoreThread::residualAlarmErr()
{
    bool res = true;
    QString str = tr("剩余电流最大报警值 ");
    bool ret = mErr->zeroLineCurAlarm();
    if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
    mLogs->updatePro(str, ret);
    return res;
}

bool Test_CoreThread::lineVolAlarmErr(int i)
{
    QString str = tr("线电压报警阈值 L%1 ").arg(i+1);
    bool ret = mErr->lineVolAlarm(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::checkAlarmErr()
{
    bool res = true, ret = true;

    for(int i=0; i<mBusData->box[mItem->addr - 1].loopNum; ++i) {
        ret = volAlarmErr(i); if(!ret) res = false;
        ret = curAlarmErr(i); if(!ret) res = false;
        ret = powAlarmErr(i); if(!ret) res = false;
        if(mItem->modeId == START_BUSBAR){
            ret = lineVolAlarmErr(i); if(!ret) res = false;
        }
    }
    if(ret) res = envAlarmErr(); if(!ret) res = false;
    if(mItem->modeId == START_BUSBAR){
        ret = hzAlarmErr(); if(!ret) res = false;
        ret = totalPowAlarmErr(); if(!ret) res = false;
        ret = zeroLineCurAlarmErr(); if(!ret) res = false;
        residualAlarmErr(); if(!ret) res = false;
    }

    return res;
}

bool Test_CoreThread::curAlarmWrite(int i)
{
    QString str = tr("电流报警阈值 L%1 写入").arg(i+1);
    bool ret = mCtrl->setCurTh(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::volAlarmWrite(int i)
{
    QString str = tr("电压报警阈值 L%1 写入").arg(i+1);
    bool ret = mCtrl->setVolTh(i);
    if(ret) str += tr("正常");
    else str += tr("错误");

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::writeAlarmTh()
{
    bool ret = true;
    sCfgDev *cth = &(mItem->cTh);
    if(cth->type) {
        for(int i=0; i<mDev->line.size; ++i) {
            ret = curAlarmWrite(i); if(!ret) break;
            ret = volAlarmWrite(i); if(!ret) break;
        }
    }

    return ret;
}

bool Test_CoreThread::factorySet()
{
    bool ret = true , res = true;
    if(mItem->modeId == START_BUSBAR && mItem->ip.addr == SNMP){
        mRead->SetInfo(mRead->getConnectModeOid(),"1");//切换成RTU模式
    }
    ret = mCtrl->eleClean();
    mRead->readDevData();
    int i = 0;
    for(; i<mBusData->box[mItem->addr-1].loopNum; ++i) {
        ret = eleErrRange0(i); if(!ret) res = false;
    }
    QString str = tr("清除电能");
    if(res) str += tr("成功"); else str += tr("L%1 失败").arg(i);
    mLogs->updatePro(str, res);

    if(mItem->modeId == START_BUSBAR && mItem->ip.addr == SNMP){
        mRead->SetInfo(mRead->getConnectModeOid(),"0");//切换成SNMP模式
        QString str = tr("对比始端箱通讯模式！");
        mLogs->updatePro(str);
        sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
        bool ret = false;
        int curValue = b->workMode;
        int expect = mItem->ip.ip_mode;
        if(curValue == expect) ret = true;
        str = tr("始端箱通讯模式实际值：%1 , 期待值：%2！").arg(mErr->changeMode(curValue)).arg(mErr->changeMode(expect));
        mLogs->updatePro(str,ret);
    }

//        if(1 != mDev->id) {
//            ret = mCtrl->factorySet();
//            str = tr("设备通讯地址");
//            if(ret) str += tr("成功"); else str += tr("失败");
//            mLogs->updatePro(str, ret);
//        }
    return res;
}

void Test_CoreThread::workResult(bool)
{
    mLogs->saveLogs();
    mLogs->updatePro(tr("测试结束"));
    mPro->step = Test_Over;
}

void Test_CoreThread::checkBaseInfo()
{
    if(mItem->modeId == START_BUSBAR) mErr->compareStartValue();
    else mErr->compareInsertValue();
}

void Test_CoreThread::workDown()
{
    mPro->step = Test_Start;
    bool ret = false;
    ret = initDev();
    if(ret) {
        ret = checkErrRange();
//        ret = true;
        if(ret) checkBaseInfo();
        if(ret) ret = checkAlarmErr();
        if(ret) ret = factorySet();
    }
    workResult(ret);
}

void Test_CoreThread::run()
{
    if(isRun) return;
    isRun = true;
    //    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    //    devip->readPduData();
    //    Dev_SiRtu *devsi = Dev_SiRtu::bulid();//rtu
    //    devsi->readPduData();
    workDown();
    mPro->step = Test_Over;
    isRun = false;
}
