#ifndef TEST_LOGS_H
#define TEST_LOGS_H
#include "dbstates.h"
#include "test_object.h"

class Test_Logs : public Test_Object
{
    Q_OBJECT
    explicit Test_Logs(QObject *parent = nullptr);
public:
    static Test_Logs *bulid(QObject *parent = nullptr);

    void saveLogs();
    bool updatePro(const QString &str, bool pass=true, int sec=1);

protected:
    bool writeLog();
    void writeLogs();
    bool appendLogItem(const QString &str, bool pass);

private:
    QList<sStateItem> mLogItems;
};

#endif // TEST_LOGS_H
