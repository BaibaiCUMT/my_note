#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QToolButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QCryptographicHash>

#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //设置首个页面
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //去边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //设置字体
    this->setFont(QFont("微软雅黑",16,QFont::Bold, false));

    connect(ui->titleWg, &TitleWidget::closeWindow,this,[=]()
    {
        //判断当前stackedWidget的页
        //设置页
        if(ui->stackedWidget->currentWidget() == ui->set_page)
        {
            //切换为登录页
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            //清空数据
            ui->name_log->clear();
            ui->password_log->clear();
        }
        //注册页
        else if (ui->stackedWidget->currentWidget() == ui->reg_page)
        {
            //切换为登录页
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            ui->name_log->clear();
            ui->password_log->clear();
        }
        //登录页
        else
        {
            this->close();
        }
    });

    connect(ui->titleWg, &TitleWidget::showSetWg, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });

    connect(ui->log_regButton, &QToolButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->reg_page);
    });


//    写Json
{
//    QJsonObject obj;

//    QJsonObject sub;
//    sub.insert("ip",QJsonValue("192.168.0.1"));
//    sub.insert("port","9999");

//    obj.insert("server",QJsonValue(sub));

//    QJsonDocument doc(obj);
//    QByteArray data = doc.toJson();

//    QFile file("temp.json");
//    file.open(QIODevice::WriteOnly);
//    file.write(data);
//    file.close();
}
//    读Json
{
//    QFile file("temp.json");
//    file.open(QIODevice::ReadOnly);
//    QByteArray data = file.readAll();
//    file.close();

//    QJsonDocument doc = QJsonDocument::fromJson(data);
//    if(doc.isObject())
//    {
//           QJsonObject obj = doc.object();
//           QJsonValue value = obj.value("server");
//           if(value.isObject())
//           {
//               QJsonObject subobj = value.toObject();
//               QString ip = subobj.value("ip").toString();
//               QString port = subobj.value("port").toString();
//               qDebug() << ip << port;
//           }
//    }
}

//    发送和接受http请求
    {
//        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

//        QNetworkRequest res;
//        res.setHeader(QNetworkRequest::UserAgentHeader,
//                      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36 Edg/111.0.1661.54");
//        res.setUrl(QUrl("http://www.baidu.com:80"));

//        QNetworkReply *reply = manager->post(res,"");

//        connect(reply, &QNetworkReply::readyRead, this, [=]()
//        {
//            QVariant str = reply->header(QNetworkRequest::LocationHeader);
//            QByteArray data = reply->readAll();
//            qDebug() << data;
//            qDebug() << str.toString();
//        });
    }

//    MD5加密
    {
//        QByteArray data = "hello world";
//        QCryptographicHash hash(QCryptographicHash::Md5);
//        hash.addData(data);
//        hash.addData("nihao");

//        QByteArray arry = hash.result();
//        qDebug() << arry;
//        arry = arry.toHex();
//        qDebug() << arry;

//        QByteArray arry2 = QCryptographicHash::hash(data,QCryptographicHash::Md5).toHex();
//        qDebug() << arry2;
    }

//    base64编码
    {
//        QByteArray base = "hello world";
//        base = base.toBase64();
//        qDebug() << base;

//        base = QByteArray::fromBase64(base);
//        qDebug() << base;
    }

}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *event)
{
    //给窗口画背景图
    QPainter p(this);
    QPixmap pixmap(":/images/login_bk.jpg");
    p.drawPixmap(0,0,this->width(),this->height(),pixmap);


}

void Login::on_reg_regButton_clicked()
{

}

//服务器设置
void Login::on_set_setButton_clicked()
{
    //获取：
    QString ip = ui->address->text();
    QString port = ui->port->text();

    //校验：
    QRegExp exp(IP_REG);
    if (!exp.exactMatch(ip))
    {
        QMessageBox::warning(this,"warning!", "ip error!");
        ui->address->clear();
        //设置焦点
        ui->address->setFocus();
        return;
    }

    exp.setPattern(PORT_REG);
    if (!exp.exactMatch(port))
    {
        QMessageBox::warning(this,"warning","port error");
        ui->port->clear();
        ui->address->setFocus();
        return;
    }

    //保存到配置文件

    //准备发送的数据

}

//保存到配置文件
void Login::saveWebInfo(QString ip, QString port, QString path)
{
    //读文件
    QFile file(path);
    bool bl = file.open(QFile::ReadOnly);
    if (bl == false)
    {
        std::cout << "fail to open conf file" << '\n';
        return;
    }
    QByteArray data = file.readAll();
    //
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isObject())
    {
        return;
    }
    QJsonObject obj = doc.object();
    QJsonObject loginobj = obj.value("login");
    QJsonValue user = loginobj.value("user");
    QJsonValue pwd = loginobj.value("pwd");

    QJsonObject pathobj = obj.value("path");

    QMap<QString, QVariant> webInfo;
    webInfo.insert("ip", ip);
    webInfo.insert("port", port);
}

