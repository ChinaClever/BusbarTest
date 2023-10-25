#ifndef HOME_THRESHOLDTABWID_H
#define HOME_THRESHOLDTABWID_H

#include "home_linetabwid.h"

class Home_ThresholdTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Home_ThresholdTabWid(int index, QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void setDataUnit(int size, sDataValue *unit);
    void setDataUnit(int size , sDataPowUnit *unit);
    void setDataUnit(sBoxData *unit);
    void appendItem(sObjectData *dev);
    void appendItem(sBoxData *dev);

    void change(double &rate , QString &suffix , QString &str , int &r);
    void addList(int idx , sRtuUshortUnit *unit ,double rate ,QString suffix ,QString str ,int r );


protected slots:
    void timeoutDone();

private:
    sObjData *mData;
    sDevData *mDev;
    sCfgItem *mItem;
    sBusData *mBusData;
    int m_index;
};

#endif // HOME_THRESHOLDTABWID_H
