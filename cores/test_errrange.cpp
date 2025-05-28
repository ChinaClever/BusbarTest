/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_errrange.h"

Test_ErrRange::Test_ErrRange(QObject *parent) : Test_Object(parent)
{
    mLogs = Test_Logs::bulid(this);
}

void Test_ErrRange::initFunSlot()
{
    mSourceDev = mPacket->getDev(0);
}

Test_ErrRange *Test_ErrRange::bulid(QObject *parent)
{
    static Test_ErrRange* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_ErrRange(parent);
    return sington;
}

bool Test_ErrRange::volErr(int id)
{
    int pass = Test_Fail;

    int value = mBusData->box[mItem->addr - 1].data.vol.value[id];
    int exValue = mSourceDev->line.vol.value[id];
    int err = exValue*(mItem->err.volErr/1000.0);
    bool ret = checkErrRange(exValue, value, err);
    if(ret) pass = Test_Pass;
    mBusData->box[mItem->addr - 1].data.vol.status[id] = pass;

    return ret;
}

bool Test_ErrRange::curErr(int id)
{
    int pass = Test_Fail;

    int value = mBusData->box[mItem->addr - 1].data.cur.value[id];
    int exValue = mSourceDev->line.cur.value[id]-2;
    if(mItem->modeId == START_BUSBAR) exValue*=15;
    int err = exValue*(mItem->err.curErr/1000.0);


    bool ret = checkErrRange(exValue, value, err);
    if(ret) pass = Test_Pass;
    mBusData->box[mItem->addr - 1].data.cur.status[id] = pass;

    return ret;
}

bool Test_ErrRange::powErr(int id)
{
    int pass = Test_Fail;
    int value = mBusData->box[mItem->addr - 1].data.pow.value[id];
    int exValue = mSourceDev->line.pow[id];
    if(mItem->modeId == START_BUSBAR) exValue*=15;
    int err = exValue * (mItem->err.powErr/1000.0);

    bool ret = checkErrRange(exValue, value, err);
    if(ret) pass = Test_Pass;
    mBusData->box[mItem->addr - 1].data.pow.status[id] = pass;

    return ret;
}

bool Test_ErrRange::temErr(int id)
{
    bool ret = false;
    int pass = Test_Fail;
    int value = mBusData->box[mItem->addr - 1].env.tem.value[id];

    if((value>0)&&(value<50))ret=true;
    if(ret) pass = Test_Pass;
    mBusData->box[mItem->addr - 1].env.tem.status[id] = pass;

    return ret;
}

bool Test_ErrRange::checkErrRange(int exValue, int value, int err)
{
    bool ret = false;
    int min = exValue - err;
    int max = exValue + err;
    if((value>=min) && (value<=max) && value) {
        ret =  true;
    } else {
        qDebug() << "value Err Range" << value << exValue << err;
    }

    return ret;
}

bool Test_ErrRange::volAlarm(int id)
{
    bool ret = true;
    sObjCfg *cth = mItem->modeId == START_BUSBAR ?(&(mItem->ip_cfg)):(&(mItem->si_cfg));
    sDataValue *unit = &(mBusData->box[mItem->addr - 1].data.vol);
    int min = cth->vol.min*cth->vol.rate;
    int max = cth->vol.max*cth->vol.rate;
    if(unit->min[id] != min) ret = false;
    if(unit->max[id] != max) ret = false;

    QString str = tr("电压阈值 L%1：下限 %2V, 上限 %3V").arg(id+1)
                      .arg(unit->min[id] / COM_RATE_VOL).arg(unit->max[id] / COM_RATE_VOL);

    QString str1 = "阈值检查";
    QString str2 = "电压阈值";
    QString str3 = tr("下限：%1V，上限：%2V").arg(cth->vol.min).arg(cth->vol.max);
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Voltage threshold L%1:  lower limit %2V,upper limit %3V").arg(id+1)
                      .arg(unit->min[id] / COM_RATE_VOL).arg(unit->max[id] / COM_RATE_VOL);

    str1 = "Threshold check";
    str2 = "Voltage threshold";
    str3 = tr("Lower limit: %1V, upper limit: %2V").arg(cth->vol.min).arg(cth->vol.max);
    mPacket->writeData_L(str1, str2, str3, str, ret);


    return ret;
}

bool Test_ErrRange::curAlarm(int id)
{    
    bool ret = true;

    sObjCfg *cth = mItem->modeId == START_BUSBAR ?(&(mItem->ip_cfg)):(&(mItem->si_cfg));
    sDataPowUnit *unit = &(mBusData->box[mItem->addr - 1].data.cur);
    int min = cth->cur.min*cth->cur.rate;
    int max = cth->cur.max*cth->cur.rate;
    if(unit->min[id] != min) ret = false;
    if(unit->max[id] != max) ret = false;

    QString str = tr("电流阈值 L%1：下限 %2A, 上限 %3A").arg(id+1)
                      .arg(unit->min[id] / COM_RATE_CUR).arg(unit->max[id] / COM_RATE_CUR);

    QString str1 = "阈值检查";
    QString str2 = "电流阈值";
    QString str3 = "与规格书一致";
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Voltage threshold L%1: lower limit %2A, upper limit %3A").arg(id+1)
              .arg(unit->min[id] / COM_RATE_CUR).arg(unit->max[id] / COM_RATE_CUR);

    str1 = "Threshold check";
    str2 = "Current threshold";
    str3 = tr("Consistent with the specifications");
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}


bool Test_ErrRange::temAlarm(int id)
{
    bool ret = true;
    sObjCfg *cth = mItem->modeId == START_BUSBAR ?(&(mItem->ip_cfg)):(&(mItem->si_cfg));
    sDataValue *unit = &(mBusData->box[mItem->addr - 1].env.tem);
    int min = cth->tem.min*cth->tem.rate;
    int max = cth->tem.max*cth->tem.rate;
    if(unit->min[id] != min) ret = false;
    if(unit->max[id] != max) ret = false;

    QString str = tr("温度传感器%1阈值：下限 %2℃, 上限 %3℃").arg(id+1)
                      .arg(unit->min[id]).arg(unit->max[id]);

    QString str1 = "阈值检查";
    QString str2 = "温度阈值";
    QString str3 = tr("默认下限：%1℃，上限：%2℃").arg(cth->tem.min /COM_RATE_TEM).arg(cth->tem.max/COM_RATE_TEM);
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Temperature sensor %1 threshold: lower limit %2℃, upper limit %3℃").arg(id+1)
              .arg(unit->min[id]).arg(unit->max[id]);

    str1 = "Threshold check";
    str2 = "Temperature threshold";
    str3 = tr("Default lower limit: %1℃, upper limit: %2℃").arg(cth->tem.min/COM_RATE_TEM).arg(cth->tem.max/COM_RATE_TEM);;
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}
bool Test_ErrRange::temEnvAlarm(int id)
{
    bool ret = true;
    sSiCfg *cth = &(mItem->si);
    sDataValue *unit = &(mBusData->box[mItem->addr - 1].env.tem);

    QString str = tr("温度传感器%1最小阈值：实际值 %2℃，期望值 %3℃").arg(id+1).arg(unit->min[id]).arg(cth->temMin[id]);
    mLogs->updatePro(str);

    if(unit->min[id] != cth->temMin[id]) ret = false;

    str = tr("温度传感器%1最大阈值：实际值 %2℃，期望值 %3℃").arg(id+1).arg(unit->max[id]).arg(cth->temMax[id]);
    mLogs->updatePro(str);
    if(unit->max[id] != cth->temMax[id]) ret = false;


    str = tr("温度传感器%1阈值：下限 %2℃, 上限 %3℃").arg(id+1)
                      .arg(unit->min[id]).arg(unit->max[id]);

    QString str1 = "阈值检查";
    QString str2 = "温度阈值";
    QString str3 = tr("默认下限：%1℃，上限：%2℃").arg(cth->temMin[id]).arg(cth->temMax[id]);
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Temperature sensor %1 threshold: lower limit %2℃, upper limit %3℃").arg(id+1)
               .arg(unit->min[id]).arg(unit->max[id]);

    str1 = "Threshold check";
    str2 = "Temperature threshold";
    str3 = tr("Default lower limit: %1℃, upper limit: %2℃").arg(cth->temMin[id]).arg(cth->temMax[id]);
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}
bool Test_ErrRange::powAlarm(int id)
{
    bool ret = true;
    sObjCfg *cth = mItem->modeId == START_BUSBAR ?(&(mItem->ip_cfg)):(&(mItem->si_cfg));
    sDataPowUnit *unit = &(mBusData->box[mItem->addr - 1].data.pow);
    uint min = cth->pow.min*cth->pow.rate;
    uint max = cth->pow.max*cth->pow.rate;
    if(unit->min[id] != min) ret = false;
    if(unit->max[id] != max) ret = false;

    QString str = tr("功率阈值 L%1：下限 %2kW, 上限 %3kW").arg(id+1)
                      .arg(unit->min[id] / 1000.000).arg(unit->max[id] / 1000.000);

    QString str1 = "阈值检查";
    QString str2 = "功率阈值";
    QString str3 = "下限：0，上限：额定电流乘以220V";
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Power threshold  L%1: lower limit %2kW, upper limit %3kW").arg(id+1)
              .arg(unit->min[id] / 1000.000).arg(unit->max[id] / 1000.000);

    str1 = "Threshold check";
    str2 = "Power threshold";
    str3 = "Lower limit: 0, upper limit: rated current multiplied by 220V";
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

bool Test_ErrRange::lineVolAlarm(int id)
{
    bool ret = true;
    sObjCfg *cth = &(mItem->ip_cfg);
    sDataValue *unit = &(mBusData->box[mItem->addr - 1].data.lineVol);
    uint min = cth->linevol.min*cth->linevol.rate;
    uint max = cth->linevol.max*cth->linevol.rate;
    if(unit->min[id] != min) ret = false;
    if(unit->max[id] != max) ret = false;

    QString str = tr("线电压阈值 L%1：下限 %2V, 上限 %3V").arg(id+1)
                      .arg(unit->min[id] / 10.0).arg(unit->max[id] / 10.0);

    QString str1 = "阈值检查";
    QString str2 = "线电压阈值";
    QString str3 = tr("下限：%1V，上限：%2V").arg(cth->linevol.min).arg(cth->linevol.max);
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Line voltage threshold L%1: lower limit %2V, upper limit %3V").arg(id+1)
              .arg(unit->min[id] / 10.0).arg(unit->max[id] / 10.0);

    str1 = "Threshold check";
    str2 = "Line voltage threshold";
    str3 = tr("Lower limit: %1V, upper limit: %2V").arg(cth->linevol.min).arg(cth->linevol.max);
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

bool Test_ErrRange::hzAlarm()
{
    bool ret = true;
    sObjCfg *cth = &(mItem->ip_cfg);
    sRtuUshortUnit *unit = &(mBusData->box[mItem->addr - 1].rate);
    uint min = cth->hz.min*cth->hz.rate;
    uint max = cth->hz.max*cth->hz.rate;
    if(unit->smin != min) ret = false;
    if(unit->smax != max) ret = false;

    QString str = tr("频率阈值：下限 %1Hz, 上限 %2Hz")
                      .arg(unit->smin / 10.0).arg(unit->smax / 10.0);

    QString str1 = "阈值检查";
    QString str2 = "频率阈值";
    QString str3 = tr("下限：%1Hz，上限：%2Hz").arg(cth->hz.min).arg(cth->hz.max);
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Frequency threshold: lower limit %1Hz, upper limit %2Hz")
              .arg(unit->smin / 10.0).arg(unit->smax / 10.0);

    str1 = "Threshold check";
    str2 = "Frequency threshold";
    str3 = tr("Lower limit: %1Hz, upper limit: %2Hz").arg(cth->hz.min).arg(cth->hz.max);
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

bool Test_ErrRange::totalPowAlarm()
{
    bool ret = true;
    sObjCfg *cth = &(mItem->ip_cfg);
    sRtuULLintUnit *unit = &(mBusData->box[mItem->addr - 1].totalPow);
    uint min = cth->totalpow.min*cth->totalpow.rate;
    uint max = cth->totalpow.max*cth->totalpow.rate;
    if(unit->imin != min) ret = false;
    if(unit->imax != max) ret = false;

    QString str = tr("总有功功率阈值：下限 %1kW，上限 %2kW")
                      .arg(unit->imin / 1000.000).arg(unit->imax / 1000.000);

    QString str1 = "阈值检查";
    QString str2 = "总有功功率阈值";
    QString str3 = "下限：0,上限：额定电流乘以3在乘以220V";
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Total active power threshold: lower limit %1kW, upper limit %2kW")
              .arg(unit->imin / 1000.000).arg(unit->imax / 1000.000);

    str1 = "Threshold check";
    str2 = "Total active power threshold";
    str3 = "Lower limit: 0, upper limit: rated current multiplied by 3 multiplied by 220V";
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

bool Test_ErrRange::zeroLineCurAlarm()
{
    bool ret = true;
    sObjCfg *cth = mItem->modeId == START_BUSBAR ?(&(mItem->ip_cfg)):(&(mItem->si_cfg));
//    sObjCfg *cth = &(mItem->ip_cfg);
    sRtuCurUintUnit *unit = &(mBusData->box[mItem->addr - 1].zeroLineCur);
    // uint min = cth->zerocur.min*cth->zerocur.rate;
    uint max = cth->zerocur.max*cth->zerocur.rate;
    // if(unit->smin != min) ret = false;
    if(unit->smax != max) ret = false;

    QString str = tr("零线电流阈值：上限 %1A")
                      .arg(unit->smax  / 1000.000);

    QString str1 = "阈值检查";
    QString str2 = "零线电流阈值";
    QString str3 = "上限：等于电流上限值";
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Zero line current threshold: upper limit %1A")
              .arg(unit->smax  / 1000.000);

    str1 = "Threshold check";
    str2 = "Zero line current threshold";
    str3 = "Upper limit: equal to the upper limit of current";
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

bool Test_ErrRange::residualAlarm()
{
    bool ret = true;
    sObjCfg *cth = &(mItem->ip_cfg);
    sRtuUshortUnit *unit = &(mBusData->box[mItem->addr - 1].reCur);
    uint max = cth->recur.max*cth->recur.rate;
    if(unit->smax != max) ret = false;

    QString str = tr("剩余电流阈值：上限 %1A")
                      .arg(unit->smax  / 1000.000);

    QString str1 = "阈值检查";
    QString str2 = "剩余电流阈值";
    QString str3 = "与规格书一致，无：0，配有：默认阈值上限0.01A";
    mPacket->writeData(str1, str2, str3, str, ret);


    str = tr("Remaining current threshold: upper limit %1A")
              .arg(unit->smax  / 1000.000);

    str1 = "Threshold check";
    str2 = "Remaining current threshold";
    str3 = "Consistent with the specifications, none: 0, equipped with: default threshold of 0.01A";
    mPacket->writeData_L(str1, str2, str3, str, ret);

    return ret;
}

void Test_ErrRange::compareInsertValue()
{
    compareInsertInfo();
    mPro->step = Test_vert;
}

void Test_ErrRange::compareInsertInfo()
{
    QString str = tr("对比插接箱基本信息！");
    mLogs->updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->baudRate;
    int expect = mItem->si.si_baud+1;
    if(curValue == expect) ret = true;
    str = tr("插接箱波特率实际值：%1 , 期待值：%2！").arg(changeBaudType(curValue)).arg(changeBaudType(expect));
    mLogs->updatePro(str,ret);

    QString str1 = "参数检查";
    QString str2 = "波特率检查";
    QString str3 = tr("默认为%1").arg(changeBaudType(expect));;

    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = "Parameter check";
    QString str5 = "Baud rate check";
    QString str6 = tr("Default is %1").arg(changeBaudType(expect));

    str = tr("Actual baud rate of plug-in box:%1 , Expected value:%2！").arg(changeBaudType(curValue)).arg(changeBaudType(expect));
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->buzzerStatus;
    expect = mItem->si.si_buzzer;
    if(curValue == expect) ret = true;
    str = tr("插接箱蜂鸣器实际值：%1 , 期待值：%2！").arg(curValue?tr("关闭"):tr("开启")).arg(expect?tr("关闭"):tr("开启"));
    mLogs->updatePro(str,ret);

    str2 = "蜂鸣器检查";
    str3 = tr("默认为%1状态").arg(expect?tr("关闭"):tr("开启"));
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of plug-in box buzzer:%1 , Expected value:%2！").arg(curValue?tr("close"):tr("open")).arg(expect?tr("close"):tr("open"));
    str5 = "Buzzer inspection";
    str6 = tr("Default is %1").arg(expect?tr("关闭"):tr("开启"));
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->alarmTime;
    expect = mItem->si.si_filter;
    if(curValue == expect) ret = true;
    str = tr("插接箱过滤次数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    mLogs->updatePro(str,ret);
    str2 = "告警滤波";
    str3 = tr("默认为%1次").arg(expect);
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of filtering times for plug-in box:%1 , Expected value:%2！").arg(curValue).arg(expect);
    str5 = "Alarm filtering";
    str6 = tr("Default is %1 times").arg(expect);
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->iOF;
    expect = mItem->si.si_iOF;
    if(curValue == expect) ret = true;
    str = tr("插接箱iOF触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "iOF报警触点检查";
    str3 = "与规格书一致";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of plug-in box iOF contact:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "IOF alarm contact inspection";
    str6 = "Consistent with the specifications";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->phaseFlag;
    expect = mItem->si.si_phaseflag;
    if(curValue == expect) ret = true;
    str = tr("插接箱输出类型实际值：%1 , 期待值：%2！").arg(curValue?tr("三相"):tr("单相")).arg(expect?tr("三相"):tr("单相"));
    mLogs->updatePro(str,ret);

    curValue = b->loopNum;
    expect = mItem->si.loopNum;
    if(curValue == expect) ret = true;
    str = tr("插接箱回路数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    mLogs->updatePro(str,ret);

    str2 = "输出类型检查";
    str3 = "与规格书一致";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Plug in box single-phase or three-phase actual value:%1 , Expected value:%2！").arg(curValue?tr("three-phase"):tr("single phase")).arg(expect?tr("three-phase"):tr("single phase"));
    str5 = "Output type check";
    str6 = "Consistent with the specifications";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;

}
void Test_ErrRange::compareEnvInfo()
{
    QString str = tr("对比温度传感器基本信息！");
    mLogs->updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->buzzerStatus;
    int expect = mItem->si.tem_buzzer;
    if(curValue == expect) ret = true;
    str = tr("温度传感器蜂鸣器实际值：%1 , 期待值：%2！").arg(curValue?tr("关闭"):tr("开启")).arg(expect?tr("关闭"):tr("开启"));
    mLogs->updatePro(str,ret);

    QString str1 = "参数检查";
    QString str2 = "蜂鸣器检查";
    QString str3 = "默认为开启状态";

    mPacket->writeData(str1, str2, str3, str, ret);

    QString str4 = "Parameter check";
    QString str5 = "Buzzer inspection";
    QString str6 = "Default to enabled state";

    str = tr("Actual value of temperature sensor buzzer:%1 , Expected value:%2！").arg(curValue?tr("close"):tr("open")).arg(expect?tr("close"):tr("open"));
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;

    curValue = b->alarmTime;
    expect = mItem->si.tem_filter;
    if(curValue == expect) ret = true;
    str = tr("温度传感器过滤次数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    mLogs->updatePro(str,ret);

    str2 = "告警滤波";
    str3 = "默认为5次";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of temperature sensor filtering times:%2！").arg(curValue).arg(expect);
    str5 = "Alarm filtering";
    str6 = "Default is 5 times";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;
}
QString Test_ErrRange::changeCurType(int index)
{
    QString str = tr("标准");
    switch(index){
    case 0:str = tr("标准");break;
    case 1:str = tr("250 A");break;
    case 2:str = tr("400 A");break;
    case 3:str = tr("650 A");break;
    case 4:str = tr("800 A");break;
    default:str = tr("标准");break;
    }
    return str;
}

QString Test_ErrRange::changeMode(int index)
{
    QString str = tr("主机地址 1 / Modbus模式");
    switch(index){
    case 0:str = tr("主机地址 0 / SNMP模式");break;
    case 1:str = tr("主机地址 1 / Modbus模式");break;
    default:str = tr("主机地址 1 / Modbus模式");break;
    }
    return str;
}

QString Test_ErrRange::changeBaudType(int index)
{
    QString str = tr("9600");
    switch(index){
    case 1:str = tr("9600");break;
    case 2:str = tr("19200");break;
    case 3:str = tr("38400");break;
    case 4:str = tr("57600");break;
    default:str = tr("9600");break;
    }
    return str;
}

void Test_ErrRange::compareStartMac()
{
    bool ret = false; QString str = "已设置过MAC地址";

    QString str1 = "Accuracy inspection";
    QString str2 =  "MAC Address Checking";

    if( mBusData->box[mItem->addr-1].mac.contains("2c:26:5f:") ){
        ret = true;
        QString info = tr("始端箱MAC Address :%1 设置成功！")
                        .arg(mBusData->box[mItem->addr-1].mac);
        mLogs->updatePro(info,ret);

        mPacket->writeData("精度检查" ,"MAC地址检查", str, mBusData->box[mItem->addr-1].mac, ret);
        str = "MAC address has already been set";
        mPacket->writeData_L(str1 ,str2, str, mBusData->box[mItem->addr-1].mac, ret);
    }else{
        QString info = tr("始端箱MAC Address :%1 设置失败！")
                        .arg(mBusData->box[mItem->addr-1].mac);
        mLogs->updatePro(info,ret);

        mPacket->writeData("精度检查" ,"MAC地址检查", str, mBusData->box[mItem->addr-1].mac, ret);
        str = "MAC address has already been set";
        mPacket->writeData_L(str1 ,str2, str, mBusData->box[mItem->addr-1].mac, ret);
    }
}

void Test_ErrRange::compareStartValue()
{
    compareStartInfo();
    compareStartMac();
}

void Test_ErrRange::compareStartInfo()
{
    QString str = tr("对比始端箱基本信息！");
    QString str1 = "参数检查";
    QString str2;
    QString str3 = "与规格书一致";

    mLogs->updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->curSpecification;
    int expect = mItem->ip.ip_curtype;
    if(curValue == expect) ret = true;
    str = tr("始端箱电流规格实际值：%1 , 期待值：%2！").arg(changeCurType(curValue)).arg(changeCurType(expect));
    mLogs->updatePro(str,ret);
    str2 = "电流规格";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of starting box current specification: %1 , Expected value: %2！").arg(changeCurType(curValue)).arg(changeCurType(expect));
    QString str4 = "Parameter check";
    QString str5 = "Current specifications";
    QString str6 = "Consistent with the specifications";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->workMode;
    expect = mItem->ip.ip_mode;
    if(curValue == expect) ret = true;
    str = tr("始端箱通讯模式实际值：%1 , 期待值：%2！").arg(changeMode(curValue)).arg(changeMode(expect));
    mLogs->updatePro(str,ret);

    str2 = "通讯模式";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of communication mode in the starting box:%1 , Expected value:%2！").arg(changeMode(curValue)).arg(changeMode(expect));
    str5 = "Communication mode";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = mPacket->share_mem_get()->boxNum;
    expect = mItem->ip.ip_num;
    if(curValue == expect) ret = true;
    str = tr("始端箱级联数目实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    mLogs->updatePro(str,ret);

    str2 = "级联数量";
    str3 = tr("默认级联数量为 %1").arg(expect);
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual number of cascaded starting boxes:%1 , Expected value:%2！").arg(curValue).arg(expect);
    str5 = "Cascade quantity";
    str6 = tr("The default cascading quantity is %1").arg(expect);
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->buzzerStatus;
    expect = mItem->ip.ip_buzzer;
    if(curValue == expect) ret = true;
    str = tr("始端箱蜂鸣器实际值：%1 , 期待值：%2！").arg(curValue?tr("关闭"):tr("开启")).arg(expect?tr("关闭"):tr("开启"));
    mLogs->updatePro(str,ret);

    str2 = "蜂鸣器开关检查";
    str3 = tr("默认为%1状态").arg((expect?tr("关闭"):tr("开启")));
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of the initial box buzzer: %1 , Expected value: %2！").arg(curValue?tr("close"):tr("open")).arg(expect?tr("close"):tr("open"));
    str5 = "Buzzer switch inspection";
    str6 = tr("Default to %1 state").arg((expect?tr("close"):tr("open")));
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->alarmTime;
    expect = mItem->ip.ip_filter;
    if(curValue == expect) ret = true;
    str = tr("始端箱过滤次数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    mLogs->updatePro(str,ret);

    str2 = "告警滤波";
    str3 = tr("默认为%1次").arg(expect);
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of filtering times for the starting box:%1 , Expected value:%2！").arg(curValue).arg(expect);
    str5 = "Alarm filtering";
    str6 = tr("Default is %1 times").arg(expect);
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->lightning;
    expect = mItem->ip.ip_lightning;
    if(curValue == expect) ret = true;
    str = tr("始端箱防雷模块实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "防雷模块检查";
    str3 = "与规格书一致";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of lightning protection module in the starting box:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "Lightning protection module inspection";
    str6 = "Consistent with the specifications";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->iOF;
    expect = mItem->ip.ip_iOF;
    if(curValue == expect) ret = true;
    str = tr("始端箱iOF辅助触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "iOF辅助触点检查";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of initial box iOF auxiliary contact:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "IOF auxiliary contact inspection";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->isd;
    expect = mItem->ip.ip_ISD;
    if(curValue == expect) ret = true;
    str = tr("始端箱ISD报警触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "ISD报警触点检查";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of ISD alarm contact in the starting box:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "ISD alarm contact inspection";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->shuntRelease;
    expect = mItem->ip.ip_shunt;
    if(curValue == expect) ret = true;
    str = tr("始端箱分励脱扣实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "分励脱扣检查";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of initial box excitation trip:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "Split excitation trip inspection";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;


    curValue = b->reState;
    expect = mItem->ip.ip_residual;
    if(curValue == expect) ret = true;
    str = tr("始端箱剩余电流实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    mLogs->updatePro(str,ret);

    str2 = "剩余电流检查";
    mPacket->writeData(str1, str2, str3, str, ret);

    str = tr("Actual value of residual current in the starting box:%1 , Expected value:%2！").arg(curValue?tr("have"):tr("nothing")).arg(expect?tr("have"):tr("nothing"));
    str5 = "Remaining current check";
    mPacket->writeData_L(str4, str5, str6, str, ret);
    ret = false;
}
