#ifndef HOME_MANUALDLG_H
#define HOME_MANUALDLG_H

#include <QDialog>
#include "home_setdlg.h"

namespace Ui {
class Home_ManualDlg;
}

class Home_ManualDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Home_ManualDlg(QWidget *parent = nullptr);
    ~Home_ManualDlg();

protected:
    bool inputCheck();

private slots:
    void on_okBtn_clicked();

private:
    Ui::Home_ManualDlg *ui;
};

#endif // HOME_MANUALDLG_H
