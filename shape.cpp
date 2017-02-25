#include "shape.h"
#include "configuration.h"
#include <QPen>
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QTime>
#include "backpanel.h"

int Shape::SHAPES[6][4][4][4] =  {
    {
        {
            1,0,0,0,
            1,0,0,0,
            1,1,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            0,1,0,0,
            0,1,0,0,
            0,0,0,0
        },
        {
            1,1,1,0,
            1,0,0,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            0,0,1,0,
            1,1,1,0,
            0,0,0,0,
            0,0,0,0
        }
    },
    {
        {
            0,1,0,0,
            1,1,1,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            0,1,0,0,
            1,1,0,0,
            0,1,0,0,
            0,0,0,0
        },
        {
            1,1,1,0,
            0,1,0,0,
            0,0,0,0,
            0,0,0,0,
        },
        {
            1,0,0,0,
            1,1,0,0,
            1,0,0,0,
            0,0,0,0
        }
    },
    {
        {
            1,1,0,0,
            1,1,0,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            1,1,0,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            1,1,0,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            1,1,0,0,
            0,0,0,0,
            0,0,0,0
        }
    },
    {
        {
            1,0,0,0,
            1,1,0,0,
            0,1,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            0,1,1,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            0,1,0,0,
            1,1,0,0,
            1,0,0,0,
            0,0,0,0
        },
        {
            0,1,1,0,
            1,1,0,0,
            0,0,0,0,
            0,0,0,0
        }
    },
    {
        {
            0,1,0,0,
            0,1,0,0,
            1,1,0,0,
            0,0,0,0
        },
        {
            1,1,1,0,
            0,0,1,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            1,1,0,0,
            1,0,0,0,
            1,0,0,0,
            0,0,0,0
        },
        {
            0,0,1,0,
            1,1,1,0,
            0,0,0,0,
            0,0,0,0
        }
    },
    {
        {
            1,0,0,0,
            1,0,0,0,
            1,0,0,0,
            1,0,0,0,
        },
        {
            1,1,1,1,
            0,0,0,0,
            0,0,0,0,
            0,0,0,0
        },
        {
            1,0,0,0,
            1,0,0,0,
            1,0,0,0,
            1,0,0,0,
        },
        {
            1,1,1,1,
            0,0,0,0,
            0,0,0,0,
            0,0,0,0
        }
    }
};

Shape::Shape(QObject *parent) :
    QObject(parent)
{
    m_x = 0;
    m_y = 0;
    m_shapeIndex = 0;
    init();
}

void Shape::init()
{
    //生成随机数
    //先指定种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int c = qrand() % 6;
//    int c = 5;
//qDebug() << "c:" << c;
    //初始化
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                m_currentShape[i][j][k] = SHAPES[c][i][j][k];
            }
        }
    }
    nextShape();
}

void Shape::setWH(int w, int h)
{
    m_width = w;
    m_height = h;
}

/**
 * 判断指定的i、j位置是否为1
 * @param i
 * @param j
 */
bool Shape::judgeShape(int i, int j) const
{
    if( m_shape[i][j] )
        return true;
    return false;
}

void Shape::nextShape()
{
    int shapeIndex =m_shapeIndex;
    m_shapeIndex = ++ shapeIndex  % 4;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m_shape[i][j] = m_currentShape[m_shapeIndex][i][j];
        }
    }
//qDebug() << "m_shapeIndex:" << m_shapeIndex;
}

void Shape::addX(int x)
{
    if( canAddX(x))
        m_x = m_x + x;
}

bool Shape::canAddX(int x)
{
    int l = m_x + x + trimLeft() * Configuration::GRID_WIDTH;
//qDebug() << "canAddX l:" << l << "\ttrimLeft:" << trimLeft() << "\ttrimRight:" << trimRight();
//qDebug() << "trimDown:" << trimDown();
//qDebug() << "m_width:" << m_width;
    int r = m_x + x + Configuration::GRID_WIDTH *  (4 - trimRight());
//qDebug() << "canAddX :" << m_x
    bool b = m_backPanel->canMoveXY(x);
//qDebug() << "b:" << b;
    if( l <= 0 || r > m_width || !b)
        return false;
    return true;
}

/**
 * @brief Shape::trimLeft
 * @return 形状最左边离边界的距离
 */
int Shape::trimLeft()
{
    int t = 4, t1 = 4;
    for(int i = 0; i < 4; i ++)
    {
        for(int j = 0;j < 4; j++)
        {
            if( m_shape[i][j] == 1)
            {
                t1 = j+1;
                if( t1 < t)
                    t = t1;
            }

        }
    }
    return t;
}

/**
 * @brief Shape::trimDown
 * @return 返回形状最下边里下边界的距离
 */
int Shape::trimDown()
{
    int t = 4, t1 = 4, r = 0;
    for(int i = 0; i < 4; i ++)
    {
        for(int j = 3;j >= 0; j--, r++)
        {
            if( m_shape[j][i] == 1)
            {
                t1 = r;
                if( t1 < t)
                {
                    t = t1;
                }
                r = 0;
                break;

            }

        }
    }
    return t;
}

/**
 * @brief Shape::trimRight
 * @return 形状最右边离边界的距离
 */
int Shape::trimRight()
{
    int t = 4, t1 = 4, r = 0;
    for(int i = 0; i < 4; i ++)
    {
        for(int j = 3;j >= 0; j--, r++)
        {
            if( m_shape[i][j] == 1)
            {
                t1 = r;
                if( t1 < t)
                {
                    t = t1;
                }
                r = 0;
                break;

            }

        }
    }
    return t;
}


bool Shape::canAddY(int y)
{
    int r = m_y + y;
    bool flag = m_backPanel->canMoveDown(y);
//qDebug() << "flag:" << flag;
    int d = m_y + y + (4 - trimDown()) * Configuration::GRID_WIDTH;
//qDebug() << "d:" << d;
    if( !flag || d > m_height)
        return false;
    return true;
}

void Shape::addY()
{
    int y = Configuration::GRID_WIDTH;
    if( canAddY(y))
        m_y = m_y + y;
    //入如果不能向下移动说明该被Barrer吸收变成障碍
    else
    {
        m_backPanel->acceptShape();
    }
}

int Shape::getX() const
{
    return m_x;
}

int Shape::getY() const
{
    return m_y;
}

void Shape::setBackPanel(BackPanel *b)
{
    m_backPanel = b;
}

void Shape::setXY(int x, int y)
{
    m_x = x;
    m_y = y;
}

/**
 * @brief Shape::getBarrer
 * @param b 将当前形状填充到Barrer类里面的障碍数组里面
 */
void Shape::getBarrer(int b[19][15])
{
//qDebug() << "getBarrer!";
    //x、y比真实的场景少1
    int x = m_x > 0 ? (m_x / Configuration::GRID_WIDTH ) : 0;
    int y = m_y / Configuration::GRID_WIDTH + 1;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if( m_shape[i][j] == 1)
                b[y+i][x+j] = m_shape[i][j];
        }
    }
}

//销毁当前Shape
void Shape::destroy()
{
    delete this;
}

void Shape::drawMe(QPainter &paint)
{
    //设置画笔画刷
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor::fromRgb(0x5b, 0x5b, 0x5b));
    paint.setPen(pen);
    paint.setBrush(QColor::fromRgb(0xff, 0x66, 0x66));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if( m_shape[i][j] == 1)
            {
                paint.drawRect(j * Configuration::GRID_WIDTH -1 + m_x,
                               i * Configuration::GRID_WIDTH - 1 + m_y,
                               Configuration::GRID_WIDTH, Configuration::GRID_WIDTH);
            }
        }
    }
}
