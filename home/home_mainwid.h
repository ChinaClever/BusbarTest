#ifndef HOMEMAINWID_H
#define HOMEMAINWID_H

#include <QWidget>
#include "serialstatuswid.h"
#include "home_workwid.h"

namespace Ui {
class Home_MainWid;
}

class Home_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_MainWid(QWidget *parent = nullptr);
    ~Home_MainWid();

signals:
    void startSig();

protected:
    void initWid();

private:
    Ui::Home_MainWid *ui;

    Home_WorkWid *mWorkWid;
    Home_EnvTabWid *mEnvTabWid;
    Home_LineTabWid *mLineTabWid;
//    Home_ThresholdTabWid *mThTabWid;
    Home_ThresholdTabWid *mThTabWid1;
    Home_ThresholdTabWid *mThTabWid2;
    Home_ThresholdTabWid *mThTabWid3;
    Home_ThresholdTabWid *mThTabWid4;
    Home_ThresholdTabWid *mThTabWid5;

};

#endif // HOMEMAINWID_H
