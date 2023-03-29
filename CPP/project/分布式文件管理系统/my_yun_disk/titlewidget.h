#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

namespace Ui {
class TitleWidget;
}

class TitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWidget(QWidget *parent = nullptr);
    ~TitleWidget();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent * ev);

signals:
    void showSetWg();
    void closeWindow();

private:
    Ui::TitleWidget *ui;
    QPoint m_pt; //存放鼠标当前位置-窗口左上角的点

    QWidget * m_parent;
};

#endif // TITLEWIDGET_H
