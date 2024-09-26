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
    mRet = -1;
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
    if( str.size() >= 17 ){
        this->mMacStr = str.right(17);
        mPro->macAddress = this->mMacStr;
    }
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
    ret = mRead->readSn();
    return ret;
}

bool Test_CoreThread::volErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<5; ++k) {
        mRead->mSource->read();
        ret = mErr->volErr(i);
        if(ret) break; else mRead->readDevData();
    }

    int a = mSour->line.vol.value[i];
    int b = mBusData->box[mItem->addr - 1].data.vol.value[i];
    float c = -1;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    QString str = tr("电压 L%1：期望电压=%2V，实测电压=%3V ，误差=%4 %").arg(i+1)
            .arg(a/COM_RATE_VOL)
            .arg(b/(COM_RATE_VOL))
            .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str += tr("正常");
    else str += tr("错误");

    QString str1 = "精度检查";
    QString str2 = "电压精度";
    QString str3 = tr("与参考源进行对比，允许误差范围在 %1%以内").arg(mItem->err.volErr / 10.0);
    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = tr("Voltage L%1: expected voltage=%2V, measured voltage=%3V, error=%4% ").arg(i+1)
            .arg(a/COM_RATE_VOL)
                      .arg(b/(COM_RATE_VOL))
                      .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str4 += tr("normal");
    else str4 += tr("error");

    str1 = "Accuracy inspection";
    str2 = "Current accuracy";
    str3 = tr("Compared with the reference source, the allowable error range is within %1%").arg(mItem->err.volErr / 10.0);
    mPacket->writeData_L(str1, str2, str3, str4, ret);

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::curErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<5; ++k) {
        ret = mErr->curErr(i);
        if(ret) break; else mRead->readDevData();
    }

    int a = mSour->line.cur.value[i]-2;
    int b = mBusData->box[mItem->addr - 1].data.cur.value[i];
    float c = -1;
    if(mItem->modeId == START_BUSBAR) a*=15;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    QString str = tr("电流 L%1，期望电流=%2A，实测电流=%3A，误差=%4 %").arg(i+1)
            .arg(a/COM_RATE_CUR).arg(b/COM_RATE_CUR)
            .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str += tr("正常");
    else {
        if(mBusData->box[mItem->addr - 1].data.cur.value[i]) {
            str += tr("错误");
        } else {
            str = tr("电流 L%1：错误，请接上负载，实测电流=0A").arg(i+1);
        }
    }

    QString str1 = "精度检查";
    QString str2 = "电流精度";
    QString str3 = tr("与参考源进行对比，允许误差范围在 %1%以内").arg(mItem->err.curErr  / 10.0);
    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = tr("Current L%1: expected current=%2A, measured current=%3A, error=%4% ").arg(i+1)
            .arg(a/COM_RATE_CUR).arg(b/COM_RATE_CUR)
                      .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str4 += tr("normal");
    else {
        if(mBusData->box[mItem->addr - 1].data.cur.value[i]) {
            str4 += tr("error");
        } else {
            str4 = tr("Current L%1, incorrect, please connect the load, measured current=0A").arg(i+1);
        }
    }

    str1 = "Accuracy inspection";
    str2 = "Current accuracy";
    str3 = tr("Compared with the reference source, the allowable error range is within %1%").arg(mItem->err.curErr  / 10.0);
    mPacket->writeData_L(str1, str2, str3, str4, ret);


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
    QString str = tr("电能 L%1：实测电能=%2Kwh").arg(i+1).arg(mBusData->box[mItem->addr-1].data.ele[i]/COM_RATE_ELE);
    bool ret = false;
    if(0 != mBusData->box[mItem->addr-1].data.ele[i]) {
        str += tr("错误");
        ret = false;
    } else {
        str += tr("正常");
        ret = true;
    }

    QString str1 = "精度检查";
    QString str2 = "电能清除";
    QString str3 = "电能为0";
    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = tr("Electric energy L%1: measured electric energy= %2Kwh ").arg(i+1).arg(mBusData->box[mItem->addr-1].data.ele[i]/COM_RATE_ELE);
    if(ret) {
        str4 += tr("normal");
    } else {
        str4 += tr("error");
    }

    str1 = "Accuracy inspection";
    str2 = "Electric energy clearing";
    str3 = "Electricity is 0";
    mPacket->writeData_L(str1, str2, str3, str4, ret);

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::powErrRange(int i)
{
    bool ret = true;
    for(int k=0; k<6; ++k) {
        ret = mErr->powErr(i);
        if(ret) break; else mRead->readDevData();
    }

    int a = mSour->line.pow[i];
    int b = mBusData->box[mItem->addr - 1].data.pow.value[i];
    float c = -1;
    if(mItem->modeId == START_BUSBAR) a*=15;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    QString str = tr("功率 L%1：期望功率=%2kW，实测功率=%3kW，误差=%4 % ").arg(i+1)
            .arg(a/COM_RATE_POW).arg(b/COM_RATE_POW)
            .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str += tr("正常");
    else str += tr("错误");

    QString str1 = "精度检查";
    QString str2 = "功率精度";
    QString str3 = tr("与参考源进行对比，允许误差范围在 %1%以内").arg(mItem->err.powErr  / 10.0);

    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = tr("Power L%1: expected power= %2kW, measured power= %3kW, error= %4% ").arg(i+1)
            .arg(a/COM_RATE_POW).arg(b/COM_RATE_POW)
                      .arg(a==0?"---":QString::number(c,'f',3));
    if(ret) str4 += tr("normal");
    else str4 += tr("error");

    str1 = "Accuracy inspection";
    str2 = "Power accuracy";
    str3 = tr("Compared with the reference source, the allowable error range is within %1%").arg(mItem->err.powErr  / 10.0);
    mPacket->writeData_L(str1, str2, str3, str4, ret);

    return mLogs->updatePro(str, ret);
}

bool Test_CoreThread::envErrRange()
{
    bool ret = true;
    for(int i = 0 ; i < 4 ; i++){
        ret = mErr->temErr(i);
        QString str = tr("传感器温度%1，实测温度= %2℃，").arg(i+1).arg(mBusData->box[mItem->addr - 1].env.tem.value[i]);
        if(ret) str += tr("正常");
        else {
            if(mBusData->box[mItem->addr - 1].env.tem.value[i]) {
                str += tr("错误");
            } else {
                str = tr("请插入传感器，实测温度 = 0");
            }
        }

        ret = mLogs->updatePro(str, ret);
    }
    return ret;
}

bool Test_CoreThread::checkErrRange()
{
    int i = 0;
    bool res = true;
    bool ret = true;
    for(; i<mBusData->box[mItem->addr-1].loopNum; ++i) {
        //        ret = volErrRange(i); if(!ret) res = false;
        ret = curErrRange(i); if(!ret) {res = false; }
        ret = eleErrRange(i); if(!ret) {res = false;}
        if(ret){ret = powErrRange(i); if(!ret) {res = false;}

        }
    }
    if(res) res = envErrRange();



    return res;
}

bool Test_CoreThread::checkVolErrRange()
{
    int i = 0;
    bool res = true, ret = true;
    for(; i<mBusData->box[mItem->addr-1].loopNum; ++i) {
        ret = volErrRange(i); if(!ret) res = false;
    }

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
    bool ret = false;
    for(int i = 0 ; i < 4 ; i++){
        QString str = tr("温度传感器%1报警阈值 ").arg(i+1);
        if(mItem->modeId == TEMPER_BUSBAR) ret = mErr->temEnvAlarm(i);
        else  ret = mErr->temAlarm(i);
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
    bool ret = mErr->residualAlarm();
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
#if ZHIJIANGINSERTBOXZERO==1
    if(ret) res = zeroLineCurAlarmErr(); if(!ret) res = false;//////////////////////////////////
#endif
    if(mItem->modeId == START_BUSBAR){
        ret = hzAlarmErr(); if(!ret) res = false;
        ret = totalPowAlarmErr(); if(!ret) res = false;
        #if ZHIJIANGINSERTBOXZERO==0
        ret = zeroLineCurAlarmErr(); if(!ret) res = false;////////////////////////////////////
        #endif
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
    return res;
}

void Test_CoreThread::clearStartEleSlot()
{
    mCtrl->eleClean();
    return;
}

bool Test_CoreThread::printer()
{
    QString method = "Integration/Busbar-Module/Execute";
    QString ip = "192.168.1.117";
    bool ret = true;
    QString str = tr("标签打印 "); QString str1;
        if(mPro->result != Test_Fail){
        sBarTend it;
        QString mPn = mItem->pn;//订单号+成品代码
        QStringList list = mPn.split("+");
        for(int i = 0; i < list.count(); i++)
        {
            if(i == 0) it.on = list.at(i);
            if(i == 1) it.pn = list.at(i);
        }

        QString mSn = mDev->devType.sn;//模块序列号
        it.sn =  mSn.remove(QRegExp("\\s"));

        int ver = get_share_mem()->box[mItem->addr-1].version;//软件版本号
        it.fw = QString::number(ver/100)+"."+QString::number(ver/10%10)+"."+QString::number(ver%10);
        it.hw = mItem->hw_ver;//暂时设置默认值

        if(it.sn.isEmpty() || it.fw.isEmpty()){
            mPro->result = Test_Fail;
            ret  = false;
            if(it.sn.isEmpty()) str += tr(" 读取到序列号SN为空 ");
            if(it.fw.isEmpty()) str += tr(" 读取到软件版本FW为空 ");
            if(it.hw.isEmpty()) str += tr(" 读取到硬件版本HW为空 ");
        }

        if(ret){
            str1 = Printer_BarTender::bulid(this)->http_post(method, ip, it);
            if(str1 == "Success") {
                ret = true;
            }else {
                str1 = Printer_BarTender::bulid(this)->http_post(method, ip, it);
                if(str1 == "Success") {
                    ret = true;
                }else ret = false;
            }
        }
        if(ret) str += tr("正常"); else str += tr("错误");
    } else str = tr("因测试未通过，标签未打印");

    return mPacket->updatePro(str, ret);
}

void Test_CoreThread::workResult(bool)
{
    mLogs->updatePro(tr("测试结束"));

    bool res = false;
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        res = printer();
        if(res){
            str += tr("通过"); mPro->uploadPassResult = 1;
        }else {
            str += tr("失败"); mPro->uploadPassResult = 0;
        }

    } else {
        res = false;
        str += tr("失败");
        mPro->uploadPassResult = 0;
    }
    mLogs->saveLogs();

    mPacket->updatePro(str, res);
    mPro->step = Test_Over;
}

void Test_CoreThread::checkBaseInfo()
{
    if(mItem->modeId == START_BUSBAR) mErr->compareStartValue();
    else mErr->compareInsertValue();
}

bool Test_CoreThread::checkVersion()
{
    QString str = tr("对比版本信息！");
    mLogs->updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->version;
    int expect = mItem->modeId == START_BUSBAR?mItem->ip.version:mItem->si.si_version;
    if(curValue == expect) ret = true;
    QString curVer = QString::number(curValue/100)+"."+QString::number(curValue/10%10)+"."+QString::number(curValue%10);
    QString expectVer = QString::number(expect/100)+"."+QString::number(expect/10%10)+"."+QString::number(expect%10);
    str = tr("版本信息实际值：%1 , 期待值：%2！").arg(curVer).arg(expectVer);
    mLogs->updatePro(str,ret);

    QString str1 = "参数检查";
    QString str2 = "软件版本";
    QString str3 = "与规格书一致";

    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Version information actual value:%1 , Expected value:%2！").arg(curVer).arg(expectVer);
    str1 = "Parameter check";
    str2 = "Software version";
    str3 = "Consistent with the specifications";
    mPacket->writeData_L(str1, str2, str3, str, ret);


    return ret;
}
bool Test_CoreThread::checkEnvVersion()
{
    QString str = tr("对比温度传感器版本信息！");
    mLogs->updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->version;
    int expect = mItem->si.tem_version;
    if(curValue == expect) ret = true;
    QString curVer = QString::number(curValue/100)+"."+QString::number(curValue/10%10)+"."+QString::number(curValue%10);
    QString expectVer = QString::number(expect/100)+"."+QString::number(expect/10%10)+"."+QString::number(expect%10);
    str = tr("版本信息实际值：%1 , 期待值：%2！").arg(curVer).arg(expectVer);
              mLogs->updatePro(str,ret);

    QString str1 = "参数检查";
    QString str2 = "软件版本";
    QString str3 = "与规格书一致";

    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Version information actual value:%1 , Expected value:%2！").arg(curVer).arg(expectVer);
    str1 = "Parameter check";
    str2 = "Software version";
    str3 = "Consistent with the specifications";
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}
void Test_CoreThread::workDown()
{
    mPro->step = Test_Start;
    bool ret = false;
    ret = initDev();
    if(ret) {
        if(mItem->modeId == TEMPER_BUSBAR){
            if(ret) ret = envErrRange();//判断设备当前温度值是否有效
            if(ret) ret = checkEnvVersion();//对比版本
            mErr->compareEnvInfo();//对比基本信息--蜂鸣器、告警滤波次数
            if(ret) ret = envAlarmErr(); if(!ret) ret = false;//对比阈值
        }else{
            ret = mRead->readDev();
            if(ret) ret = checkErrRange();
            else mPro->result = Test_Fail;
            if(ret) ret = checkVersion();
            if(ret) checkBaseInfo();
            if(ret) ret = checkAlarmErr();//检查报警阈值

            emit noLoadSig();
            while(true){
                sleep(1);
                if(mRet == 1 || mRet == 2) break;
            }
            if(mRet == 1 || mRet == 2) {ret = checkVolErrRange();if(ret) ret = factorySet();}
            mRet = -1;
        }
    }
    if(!ret) mPro->result = Test_Fail;
    workResult(ret);
}
void Test_CoreThread::noloadHomeSlot(int ret)
{
    mRet = ret;
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
