/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_pack.h"


Json_Pack::Json_Pack(QObject *parent)
{
    mPro = sDataPacket::bulid()->getPro();
}

Json_Pack *Json_Pack::bulid(QObject *parent)
{
    static Json_Pack* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Pack(parent);
    }
    return sington;
}

void Json_Pack::head(QJsonObject &obj)
{
    QDateTime t = QDateTime::currentDateTime();
    mPro->testTime = QString::number(QDateTime::fromString(mPro->testStartTime,"yyyy-MM-dd HH:mm:ss").secsTo(t));
    mPro->testTime += "s";

    obj.insert("orderId", mPro->order_id);
    obj.insert("productSn", mPro->product_sn);
    obj.insert("moduleType", mPro->module_type);
    obj.insert("moduleSn", mPro->module_sn);
    obj.insert("softVersion", mPro->softwareVersion);
    obj.insert("languageSelect", mPro->language);

    obj.insert("testStep", mPro->test_step);
    obj.insert("testItem", mPro->test_item);
    obj.insert("testRequire", mPro->test_require);
    obj.insert("testResult", mPro->test_result);
    obj.insert("judgeResult", mPro->judge_result);
    obj.insert("testTime", mPro->testTime);

    // pduInfo(obj);
}

void Json_Pack::pduInfo(QJsonObject &obj)
{
    objData(obj);
}



int Json_Pack::objData(QJsonObject &obj)
{
    QJsonArray jsonArray;
    int num = mPro->uploadPass.size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("no",QString::number(i+1));
        subObj.insert("name", mPro->itemName.at(i));
        subObj.insert("result", mPro->uploadPass.at(i)?1:0);

        jsonArray.append(subObj);
    }
    obj.insert("testStep" ,QJsonValue(jsonArray));

    return num;
}

void Json_Pack::getJson(QJsonObject &json , QByteArray &ba)
{
    head(json);
    QJsonDocument jsonDoc(json);
    ba = jsonDoc.toJson();
}
void Json_Pack::http_post(const QString &method, const QString &ip, int port)
{
    QJsonObject json; head(json);
    qDebug()<<"http_post"<<json;
    AeaQt::HttpClient http;
    http.clearAccessCache();
    http.clearConnectionCache();
    QString url = "http://%1:%2/%3";
    http.post(url.arg(ip).arg(port).arg(method))
        .header("content-type", "application/json")
        .onSuccess([&](QString result) {qDebug()<<"result"<<result;})
        .onFailed([&](QString error) {qDebug()<<"error"<<error;})
        .onTimeout([&](QNetworkReply *) {qDebug()<<"http_post timeout";}) // 超时处理
        .timeoutMs(200) // 200ms超时
        .block()
        .body(json)
        .exec();
}

//bool Json_Build::saveJson( QJsonObject &json)
//{
//    QJsonDocument jsonDoc(json);
//    QByteArray ba = jsonDoc.toJson();
//    QString path = CfgCom::bulid()->pathOfData("pdu_id.json");
//    QFile file(path);
//    bool ret = false;
//    if(file.exists())//文件存在则不需要再写
//    {
//        ret = true;
//    }
//    else//文件不存在则写入初始数据
//    {
//        ret = file.open(QIODevice::WriteOnly);
//        if(ret) {
//            file.write(ba);
//            file.close();
//        } else {
//            qDebug() << "write json file failed";
//        }
//    }

//    return ret;
//}

