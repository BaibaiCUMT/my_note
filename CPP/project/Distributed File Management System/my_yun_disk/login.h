#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

//正则表达式
//#define USER_REG        "^[a-zA-Z0-9]{2,16}$"
//#define USER_PWD        "^[a-zA-Z0-9]{2,16}$"
#define IP_REG  "^(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define PORT_REG "^(?:[1-9]\d{0,3}|[1-5]\d{4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$"

//文件
#define CONFFILE    "conf/cfg.json"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_3_clicked();

    void on_reg_regButton_clicked();

    void on_set_setButton_clicked();

    //保存配置文件的信息
    void saveWebInfo(QString ip, QString port, QString path);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
