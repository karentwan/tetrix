#include "barrier.h"
#include "shape.h"
#include <QDebug>
#include <QPainter>
#include "configuration.h"
#include <QPen>
#include <QThread>
#include "backpanel.h"


int Barrier::ROW = 19;

int Barrier::COLUMN = 15;

Barrier::Barrier(QObject *parent) :
    QObject(parent),
    m_mutex()
{
    init();
}

void Barrier::init()
{
    //需要先初始化，不然就是随机数
    for(int i = 0; i < 19; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            m_barrers[i][j] = 0;
        }
    }
}

/**
 * 消除满行，如果有满行则消除，如果没有就不消除
 */
void Barrier::haveFullLine()
{

    for(int i = 0; i < 19; i++)
    {
        bool flag = true;
        for(int j = 0; j < 15; j++)
        {
            if( m_barrers[i][j] == 0)
            {
                flag = false;
                break;
            }
        }
        if( flag )
        {
            //休眠
//            QThread::msleep(500);
            deleteFullLine(i);
        }
    }
}

void Barrier::setBackPanel(BackPanel *b)
{
    m_parent = b;
}

/**
 * 删除满行
 * @param i 行坐标
 */
void Barrier::deleteFullLine(int i)
{
//qDebug() << "delete Full line";
    //删除满行
    for(int j = 0; j < 15; j++)
    {
        m_barrers[i][j] = 0;
    }
    //将上面的内容往下面移动
    for(int k = i; k > 0; k--)
    {
        for(int j = 0; j < 15; j++)
        {
            m_barrers[k][j] = m_barrers[k-1][j];
        }
    }
    //删除满行之后增加分数
    m_parent->addScore();
}

/**
 * 绘制障碍
 * @param paint
 */
void Barrier::drawMe(QPainter &paint)
{
//qDebug() << "barrier draw me !";
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor::fromRgb(0x5b, 0x5b, 0x5b));
    paint.setPen(pen);
    paint.setBrush(QColor::fromRgb(0x40, 0x40, 0x40));
    for(int i = 0; i < ROW; i ++ )
    {
        for(int j = 0; j < COLUMN; j++)
        {
            //如果数组里面的书等于1便绘制出来
            if( m_barrers[i][j] == 1)
            {
                 paint.drawRect(j * Configuration::GRID_WIDTH -1,
                                i * Configuration::GRID_WIDTH - 1,
                                Configuration::GRID_WIDTH, Configuration::GRID_WIDTH);
            }
        }
    }
}

/**
 * @brief 判断y方向是否有障碍，这个方法应该是线程安全的
 * @param y
 * @return true 表示有障碍，false表示没有障碍
 */
bool Barrier::hasBrrerFromY(int x, int y, Shape *s)
{
//qDebug() << "hasBarrerFromY x:" << x << "\ty:" << y;
    m_mutex.lock();//加锁
    for(int i = 0; i < 4; i++)
    {
        for(int j =0; j < 4; j++)
        {
            if( m_barrers[y + i + 1][ x + j] == 1 && s->judgeShape(i, j))
            {
                m_mutex.unlock();
                return true;
            }

        }
    }
    //解锁
    m_mutex.unlock();
    return false;
}

/**
 * @brief Barrier::hasBarrerFromX
 * @param x 列坐标
 * @param y 行坐标
 * @param s Shape形状
 * @return true 表示有障碍， false表示没有障碍
 */
bool Barrier::hasBarrerFromX(int x, int y, Shape *s)
{
//qDebug() << "x:" << x << "\ty:" << y;
    for( int i = 0; i < 4; i++) //列
    {
        for(int j = 0; j < 4; j++)//行
        {
            if( m_barrers[j+y][i+x] == 1 && s->judgeShape(j, i))
            {
//qDebug() << "j+y:" << j+y << "\ti+x:" << i+x;
//qDebug() << "m_barrers[j+y][i+x]:" << m_barrers[j+y][i+x];
                return true;
            }
        }
    }
    return false;
}


void Barrier::clear()
{
    //清空数组
    for(int i = 0; i < 19; i++)
    {
        for(int j = 0; j <15; j++)
        {
            m_barrers[i][j] = 0;
        }
    }
}

void Barrier::accept(Shape *shape)
{
//qDebug() << "accept";
   shape->getBarrer(m_barrers);
   //接受之后判断是否游戏结束
   for(int i = 0; i < 15; i++)
   {
       if( m_barrers[0][i] == 1)
       {
           m_parent->gameOver();
           return;
       }
   }
}
