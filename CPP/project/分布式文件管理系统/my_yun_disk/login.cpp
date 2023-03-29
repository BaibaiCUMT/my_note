#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QToolButton>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

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
