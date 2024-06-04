#ifndef TEST_DATAREAD_H
#define TEST_DATAREAD_H

#include "test_devctrl.h"

class Test_DevRead : public Test_Object
{
    Q_OBJECT
    explicit Test_DevRead(QObject *parent = nullptr);
public:
    static Test_DevRead *bulid(QObject *parent = nullptr);

    bool readSn();
    bool readDev();
    bool readHub();
    bool readNet();
    bool checkNet();
    bool readDevData();
    QString getConnectModeOid();
    bool SetInfo(QString o , QString val);
    Dev_Source *mSource;

protected:
    void run();
    bool readSnmp();

    bool checkSiLine();
    bool checkIpLine();
    bool checkIpVersion();

protected slots:
    void initFunSlot();

private:
    Dev_Object *mRtu;
    Dev_SiRtu *mSiRtu;
    Dev_IpRtu *mIpRtu;
    Sn_SerialNum *mSn;
    Dev_IpSnmp *mIpSnmp;

    Test_Logs *mLogs;
    sCfgItem *mItem;
};

#endif // TEST_DATAREAD_H
