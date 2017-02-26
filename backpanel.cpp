#include "backpanel.h"
#include "configuration.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>


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
}

void BackPanel::suspendOrRun()
{
    m_thread->suspendOrRun();
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
    }
    update();
}

void BackPanel::setGameLevel(int g)
{
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
//qDebug() << "m_thread destroy!";
        delete m_thread;
        m_thread = NULL;
    }
}

