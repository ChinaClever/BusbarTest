/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"
#include "json_pack.h"

sDataPacket::sDataPacket()
{
    pro = new sProgress();
    for(int i=0; i<DEV_NUM; ++i) {
        dev[i] = new sDevData;
        clear(i);
    }
}

void sDataPacket::clear(int id)
{
    sDevData *ptr = dev[id];
    ptr->devType.ac = 1;
    ptr->devType.ip[0] = 0;
    ptr->devType.devType = 0;
    ptr->devType.version = 0;
    ptr->devType.sn.clear();
    ptr->devType.dev_type.clear();
    memset(&(ptr->line), 0, sizeof(sObjData));
    memset(&(ptr->env), 0, sizeof(sEnvData));

    pro->step = 0;
    pro->result = 0;
    pro->pass.clear();
    pro->itPass.clear();
    pro->item.clear();
    pro->status.clear();
    pro->macAddress.clear();


    // pro->testStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    pro->testEndTime.clear();
    pro->no.clear();
    pro->itemName.clear();
    pro->uploadPass.clear();

    pro->softwareVersion.clear();
    pro->product_sn.clear();
    pro->order_id.clear();
    pro->module_type.clear();
    pro->module_sn.clear();
    pro->testTime.clear();
    pro->test_step.clear();
    pro->test_item.clear();
    pro->test_require.clear();
    pro->test_result.clear();
    pro->judge_result = 0;
    pro->language = 0;
    pro->Service.clear();
    pro->type = 0;
}


sDataPacket *sDataPacket::bulid()
{
    static sDataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new sDataPacket();
    return sington;
}

void sDataPacket::init()
{
    clear();
    pro->step = Test_Fun;
    pro->result = Test_Info;
    pro->startTime = QTime::currentTime();
    pro->testStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

bool sDataPacket::updatePro(const QString &str, bool pass, int sec)
{
    pro->time = QTime::currentTime().toString("hh:mm:ss");
    pro->pass << pass;
    pro->itPass << pass;

    pro->item << str;
    pro->status << str;

    if(pass) pass = delay(sec);
    else pro->result = Test_Fail;
    // if(!pass) BaseLogs::bulid()->appendLogItem(str, pass);

    return pass;
}

bool sDataPacket::writeData(const QString &str1, const QString &str2, const QString &str3, const QString &str4, bool pass, int sec)
{
    pro->time = QTime::currentTime().toString("hh:mm:ss");
    pro->test_step = str1;
    pro->test_item = str2;
    pro->test_require = str3;
    pro->test_result = str4;
    pro->judge_result = pass;
    pro->language = 0;

    if(pro->type == 1) {
        pro->module_type = "始端箱";
    } else if(pro->type == 2) {
        pro->module_type = "插接箱";
    }

    if(pro->online) Json_Pack::bulid()->http_post("admin-api/bus/moduleTest",pro->Service);//全流程才发送记录(http)

    return pass;
}

bool sDataPacket::writeData_L(const QString &str1, const QString &str2, const QString &str3,  const QString &str4,bool pass, int sec)
{
    pro->time = QTime::currentTime().toString("hh:mm:ss");
    pro->test_step = str1;
    pro->test_item = str2;
    pro->test_require = str3;
    pro->test_result = str4;
    pro->judge_result = pass;
    pro->language = 1;

    if(pro->type == 1) {
        pro->module_type = "Busbar feeder box";
    } else if(pro->type == 2) {
        pro->module_type = "Busbar tap-off box";
    }

    if(pro->online) Json_Pack::bulid()->http_post("admin-api/bus/moduleTest",pro->Service);//全流程才发送记录(http)

    return pass;
}

bool sDataPacket::delay(int s)
{
    bool ret = true;
    for(int i=0; i<s; ++i) {
        if((pro->step < Test_Over)  || (pro->step > Test_End)){
            QThread::msleep(100);
        } else {
            ret = false;
            break;
        }
    }

    return ret;
}

/**
 * 获取共享内存
 */

sBusData *sDataPacket::share_mem_get()
{
    static sBusData *ptr = nullptr;
    if(!ptr) {
        ptr = new sBusData();
    }

    return ptr;
}

/**
 * 共享内存初始化
 */
void sDataPacket::share_mem_init()
{
    sBusData *shared = share_mem_get();//指向shm
}

/**
 * 把共享内存从当前进程中分离
 */
void sDataPacket::share_mem_free()
{
    sBusData *shm = share_mem_get();
    delete shm;
}

/**
 * 删除共享内存
 */
void sDataPacket::share_mem_del()
{
    share_mem_free();
}


/***
  * 获取共享内存
  */
sBusData *get_share_mem()
{
    static sBusData *shm = NULL;
    if(shm == NULL) {
        shm = sDataPacket::bulid()->share_mem_get();
        sDataPacket::bulid()->share_mem_init();
    }

    return shm;
}
