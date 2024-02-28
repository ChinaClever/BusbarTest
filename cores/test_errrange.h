#ifndef TEST_ERRRANGE_H
#define TEST_ERRRANGE_H

#include "test_devread.h"

class Test_ErrRange : public Test_Object
{
    Q_OBJECT
    explicit Test_ErrRange(QObject *parent = nullptr);
public:
    static Test_ErrRange *bulid(QObject *parent = nullptr);

    bool volErr(int id);
    bool curErr(int id);
    bool powErr(int id);

    bool temErr(int id);

    bool volAlarm(int id);
    bool curAlarm(int id);
    bool temAlarm(int id);
    bool temEnvAlarm(int id);
    bool powAlarm(int id);

    bool lineVolAlarm(int id);
    bool hzAlarm();
    bool totalPowAlarm();
    bool zeroLineCurAlarm();
    bool residualAlarm();

    void compareInsertValue();
    void compareStartValue();
    QString changeMode(int index);
    void compareEnvInfo();

protected:
    bool checkErrRange(int exValue, int value, int err);
    void compareInsertInfo();

    void compareStartInfo();
    void compareStartMac();
    QString changeBaudType(int index);
    QString changeCurType(int index);

protected slots:
    void initFunSlot();

private:
    sDevData *mSourceDev;
    Test_Logs *mLogs;
};

#endif // TEST_ERRRANGE_H
