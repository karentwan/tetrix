#include "backpanel.h"
#include "configuration.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include "widget.h"
#include <QFont>


BackPanel::BackPanel(QWidget *parent) :
    QWidget(parent),
    m_barrier(new Barrier)
{
    init();
    initStyle();
    setFocusPolicy(Qt::StrongFocus);
    //获得焦点
    setFocus();
    m_start = false;
    m_parent = static_cast<Widget *>(parent);
    m_score = 0;
    //游戏水平默认为1
    m_gameLevel = 7;
    m_barrier->setBackPanel(this);
    m_gameOver = false;
}

void BackPanel::suspendOrRun()
{
    m_thread->suspendOrRun();
}

/**
 * 增加分数
 */
void BackPanel::addScore()
{
//qDebug() << "BackPanel::addScore invoke!";
    int score = (8 - m_gameLevel) * 10;
    m_score += score;
    m_parent->setScore(m_score);
}

void BackPanel::stopGame()
{
    setStart(false);
    m_thread->setRun(false);
    //清空数组
    m_barrier->clear();

    m_shape->destroy();
    m_shape = NULL;

    //删除线程
//    delete m_thread;
//    m_thread = NULL;
    update();
}

void BackPanel::setStart(bool s)
{
    m_start = s;
}

QSize BackPanel::sizeHint() const
{
    int w = Configuration::GRID_WIDTH * 15;
    int h = Configuration::GRID_WIDTH * 19;
    return QSize(w, h);
}

void BackPanel::init()
{
    m_back = true;
    m_net = true;
    //设置默认背景颜色
    m_backColor = QColor::fromRgb(205, 219, 255);
    m_netColor = QColor::fromRgb(0xc0, 0xc0, 0xc0);
    //初始化就有一个Shape
    m_shape = new Shape;
    //让Shape保持BackPanel的指针
    m_shape->setBackPanel(this);
    //开启线程
    m_thread = new Thread;
    m_thread->setBackPanel(this);
}

void BackPanel::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
//qDebug() << "resize Event!";
//    m_shape->setWH(geometry().width(), geometry().height());
//    int h = geometry().height();
    m_shape->setWH(geometry().width(), 475);
}

void BackPanel::initStyle()
{

}

/**
 * 恢复默认设置
 */
void BackPanel::reset()
{
    m_backColor = QColor::fromRgb(205, 219, 255);
    m_netColor = QColor::fromRgb(0xc0, 0xc0, 0xc0);
    m_net = true;
    m_back = true;
    update();
}

/**
 * 判断shape是否能够往下移动
 * @return true 表示能移动，false表示不能移动
 */
bool BackPanel::canMoveDown(int y)
{
    //计算形状所占的最长的距离
    int d =4 - m_shape->trimDown();
    int dy = (m_shape->getY())/ Configuration::GRID_WIDTH;
    int x = m_shape->getX() / Configuration::GRID_WIDTH;
//qDebug() << "canMoveDown x:" << x;
//qDebug() << "getY:" << m_shape->getY() << "\ty:" << y;
    bool flag = m_barrier->hasBrrerFromY(x, dy, m_shape);
    return !flag;
}

/**
 * 游戏结束
 */
void BackPanel::gameOver()
{
qDebug() << "game over";
    m_thread->setRun(false);
    m_gameOver = true;
}

bool BackPanel::canMoveXY(int x)
{
    int dy = m_shape->getY()/ Configuration::GRID_WIDTH;
    int dx = (m_shape->getX() + x)/ Configuration::GRID_WIDTH;
    return !m_barrier->hasBarrerFromX(dx, dy, m_shape);
}

/**
 * 线程安全的移动坐标
 * @param x
 * @param y
 */
void BackPanel::moveXY(int x, int y)
{
    m_mutex.lock();
    if( x != -1)
    {
        m_shape->addX(x);
    } else
    {
        m_shape->addY();
    }
    m_mutex.unlock();
}


/**
 * 形状运行
 */
void BackPanel::shapeRun()
{
//qDebug() << "shape run...";
//qDebug() << "m_shape:" << m_shape;
//    m_shape->addY();
    moveXY(-1, Configuration::GRID_WIDTH);
    update();
}

/**
 * 让Barrer接收Shape
 */
void BackPanel::acceptShape()
{
    m_barrier->accept(m_shape);
    m_shape->destroy();
    m_shape = new Shape;

    m_shape->setBackPanel(this);
    m_shape->setWH(geometry().width(), 475);

    //判断是否有满行
    m_barrier->haveFullLine();
}

void BackPanel::setBackColor(QColor c)
{
    m_backColor = c;
}

void BackPanel::setBack(bool b)
{
    m_back = b;
    update();
}

void BackPanel::setNet(bool n)
{
    m_net = n;
    update();
}

void BackPanel::setNetColor(QColor c)
{
    m_netColor = c;
}

void BackPanel::paintEvent(QPaintEvent *e)
{
    if( !m_start )
        return;
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    if( m_back )
        drawBack(paint);
    if( m_net )
        drawNet(paint);
    drawChild(paint);

    //如果游戏结束，那么绘制结束字样
    if( m_gameOver )
        drawGameOver(paint);

}

//绘制结束时的样式
void BackPanel::drawGameOver(QPainter &paint)
{
    paint.save();
    //设置画笔颜色为红色
    paint.setPen(Qt::red);
    //设置画笔的字体
    QFont font;
    font.setPointSize(30);

    paint.setFont(font);

    paint.drawText(rect(), Qt::AlignCenter, QStringLiteral("GAME OVER!SCORE:%1").arg(m_score));

    paint.restore();
}

void BackPanel::startGame()
{
    if( !m_thread )
    {
        m_thread = new Thread;
        m_thread->setBackPanel(this);
    }
    m_thread->start();

    m_start = true;
    update();
}


void BackPanel::keyPressEvent(QKeyEvent *e)
{
    if( !m_shape )
        return;
    int k = e->key();
    switch( k )
    {
    case Qt::Key_Left:
//        m_shape->addX(- Configuration::GRID_WIDTH);
        moveXY(- Configuration::GRID_WIDTH, -1);
        break;
    case Qt::Key_Right:
//        m_shape->addX(Configuration::GRID_WIDTH);
        moveXY(Configuration::GRID_WIDTH, -1);
        break;
    case Qt::Key_Up:
        m_shape->nextShape();
        break;
    case Qt::Key_Down:
//        m_shape->addY();
        moveXY(-1, Configuration::GRID_WIDTH);
        break;
        //回车键
    case Qt::Key_Enter:     //小键盘的enter键
    case Qt::Key_Return:     //回车键
//        qDebug() << "enter !";
        m_parent->suspend();
        break;

    }
    update();
}

void BackPanel::setGameLevel(int g)
{
    //保存游戏水平
    m_gameLevel = g / 100;
    m_thread->setSleepTime(g);
}

void BackPanel::drawBack(QPainter &paint)
{
    paint.save();
    int w = geometry().width();
    int h = geometry().height();
    paint.setBrush(m_backColor);
    paint.drawRect(0, 0, w, h);
    paint.restore();
}

/**
 * 绘制网格线
 * @param paint
 */
void BackPanel::drawNet(QPainter &paint)
{
//qDebug() << "drawNet";
    paint.save();
    QPen pen;
    pen.setColor(m_netColor);
    pen.setWidth(1);
    paint.setPen(pen);
    int w = geometry().width();
    int h = geometry().height();
    //绘制行
    for(int i = 0; i < Configuration::ROW; i++)
    {
        paint.drawLine(0, Configuration::GRID_WIDTH * i, w, Configuration::GRID_WIDTH * i);
    }
    //绘制列
    for(int i = 0; i < Configuration::COLUMN; i++)
    {
        paint.drawLine(Configuration::GRID_WIDTH * i, 0, Configuration::GRID_WIDTH * i, h);
    }
    paint.restore();
}

void BackPanel::drawChild(QPainter &paint)
{
    paint.save();
    m_barrier->drawMe(paint);
    //如果当前有Shape那么可以绘制Shape形状
    if( m_shape )
    {
        m_shape->drawMe(paint);
    }
    paint.restore();
}

BackPanel::~BackPanel()
{
    if( m_barrier )
    {
        delete m_barrier;
        m_barrier = NULL;
    }
    if( m_shape )
    {
        delete m_shape;
        m_shape = NULL;
    }
    if( m_thread )
    {
qDebug() << "m_thread destroy!";
        if( m_thread->isRunning() )
            m_thread->quit();
        delete m_thread;
        m_thread = NULL;
    }
}

