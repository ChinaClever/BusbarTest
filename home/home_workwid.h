#ifndef HOME_WORKWID_H
#define HOME_WORKWID_H

#include <QWidget>
#include "home_manualdlg.h"
#include "test_corethread.h"
#include "home_printdlg.h"

namespace Ui {
class Home_WorkWid;
}

class Home_WorkWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_WorkWid(QWidget *parent = nullptr);
    ~Home_WorkWid();

signals:
    void startSig();

protected:
    void createWid();
    void initLayout();
    void insertText();
    void setTextColor();

    QString getTime();
    void updateWid();
    void updateTime();
    void updateResult();
    void updateCnt();

    bool initSerial();
    bool initWid();

private slots:
    void timeoutDone();
    void on_setBtn_clicked();
    void on_startBtn_clicked();
//    void on_readBtn_clicked();

    void on_printBtn_clicked();

private:
    Ui::Home_WorkWid *ui;

    uint mId, mFirst;
    QTimer *timer;
    sDevData * mDev;
    sProgress *mPro;
    sCfgItem *mItem;
    sDataPacket *mPacket;
    Home_SetDlg *mSetDlg;
    Home_PrintDlg *mPrintDlg;
    Home_ManualDlg *mManualDlg;
    Test_CoreThread *mCoreThread;
};

#endif // HOME_WORKWID_H
