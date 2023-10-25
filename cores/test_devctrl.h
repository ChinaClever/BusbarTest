#ifndef TEST_DEVCTRL_H
#define TEST_DEVCTRL_H

#include "test_network.h"

class Test_DevCtrl : public Test_Object
{
    Q_OBJECT
    explicit Test_DevCtrl(QObject *parent = nullptr);
public:
    static Test_DevCtrl *bulid(QObject *parent = nullptr);

    bool setCurTh(int i);
    bool setVolTh(int i);
    bool factorySet();
    bool eleClean();

protected:
    bool initDev();

protected slots:
    void initFunSlot();

private:
    Ctrl_Object *mRtu;
    Ctrl_SiRtu *mSiRtu;
    Ctrl_IpRtu *mIpRtu;
};

#endif // TEST_DEVCTRL_H
