#ifndef HOME_LINETABWID_H
#define HOME_LINETABWID_H

#include "comtablewid.h"
#include "datapacket.h"
#include "config.h"

class Home_LineTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Home_LineTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void appendItem(sObjectData *dev);

protected slots:
    void timeoutDone();

private:
    sObjData *mData;
    sDevData *mDev;
    sCfgItem *mItem;
    sBusData *mBusData;
};

#endif // HOME_LINETABWID_H
