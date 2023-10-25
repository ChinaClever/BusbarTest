#ifndef TD_STATUSTABWID_H
#define TD_STATUSTABWID_H

#include "comtablewid.h"
#include "datapacket.h"

class Td_StatusTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_StatusTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void appendItem();

public slots:
    void startSlot();

protected slots:
    void timeoutDone();

private:
    sProgress *mPro;
};

#endif // TD_STATUSTABWID_H
