#ifndef TEST_OBJECT_H
#define TEST_OBJECT_H

#include "ctrl_sirtu.h"

class Test_Object : public QThread
{
    Q_OBJECT
public:
    explicit Test_Object(QObject *parent = nullptr);
    ~Test_Object();

protected slots:
    virtual void initFunSlot(){}

protected:
    bool isRun;
    sDevType *mDt;
    sDevData *mDev;
    sDevData *mSour;
    sCfgItem *mItem;
    sProgress *mPro;
    sDataPacket *mPacket;
    sBusData *mBusData;
};

#endif // TEST_OBJECT_H
