#include "titlewidget.h"
#include "ui_titlewidget.h"
#include <QMouseEvent>
#include <QToolButton>


TitleWidget::TitleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWidget)
{
    ui->setupUi(this);

    m_parent = parent;

    connect(ui->set, &QToolButton::clicked, this, [=]()
    {
        //发送自定义信号
        emit showSetWg();
    });

    connect(ui->min, &QToolButton::clicked, this, [=]()
    {
        m_parent->showMinimized();
    });

    connect(ui->close, &QToolButton::clicked, this, [=]()
    {
        emit closeWindow();
    });
}

TitleWidget::~TitleWidget()
{
    delete ui;
}

void TitleWidget::mouseMoveEvent(QMouseEvent *event)
{
    //只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        //窗口锁着鼠标移动
        //窗口左上角点 = 鼠标当前位置 - 差值
        m_parent->move(event->globalPos() - m_pt);
    }
}

void TitleWidget::mousePressEvent(QMouseEvent *ev)
{
    //如果鼠标左键按下
    if (ev->button() == Qt::LeftButton)
    {
        //求差值：鼠标当前位置 - 窗口左上角点
        m_pt = ev->globalPos() - m_parent->geometry().topLeft();
    }
}
