#ifndef BACKPANEL_H
#define BACKPANEL_H

#include <QWidget>
#include "barrier.h"
#include "shape.h"
#include "thread.h"
#include <QMutex>

class Widget;

class BackPanel : public QWidget
{
    Q_OBJECT
public:
    explicit BackPanel(QWidget *parent = 0);

    void init();

    void initStyle();
    //重绘事件
    void paintEvent(QPaintEvent *e);
    //键盘事件
    void keyPressEvent(QKeyEvent *e);

    void setBackColor(QColor c);

    void setBack(bool b);

    void setNet(bool n);

    void setNetColor(QColor c);

    void reset();

    void resizeEvent(QResizeEvent *e);

    void acceptShape();

    bool canMoveDown(int y);

    void shapeRun();

    void startGame();
    //暂停或者运行
    void suspendOrRun();
    //停止游戏
    void stopGame();

    void setStart(bool s);
    //设置游戏水平
    void setGameLevel(int g);
    //游戏结束
    void gameOver();

    void addScore();

    /*
     * 判断Shape能不能向左右两边移动
     */
    bool canMoveXY(int x);

    void moveXY(int x, int y);

    ~BackPanel();

signals:

public slots:

private:
    void drawNet(QPainter &paint);

    void drawBack(QPainter &paint);

    void drawChild(QPainter &paint);

    void drawGameOver(QPainter &paint);


private:
    bool m_net;         //显示网格
    bool m_back;        //显示背景
    QColor m_netColor;  //网格颜色
    QColor m_backColor; //背景颜色

    Barrier *m_barrier; //障碍
    Shape *m_shape;     //形状
    Thread *m_thread;
    bool m_start;       //游戏是否开始

    QMutex m_mutex;     //线程锁

    Widget *m_parent;   //Widget类

    int m_gameLevel;    //游戏水平

    int m_score;        //游戏分数

    bool m_gameOver;    //游戏是否结束

};

#endif // BACKPANEL_H
