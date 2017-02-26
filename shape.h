#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QMutex>


class QPainter;
class BackPanel;

class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);

    void init();

    void drawMe(QPainter &paint);

    void setXY(int x, int y);
    //设置backpanel的长和高
    void setWH(int w, int h);

    void addX(int x);
    //判断是否能够添加x坐标
    bool canAddX(int x);
    //判断是否能够添加y坐标
    bool canAddY(int y);

    int getX() const;

    int getY() const;

    int trimLeft();

    int trimRight();

    int trimDown();

    void addY();

    void addY(int y);

    void destroy();

    void nextShape();

    void setBackPanel(BackPanel *b);

    void getBarrer(int b[19][15]);

    bool judgeShape(int i, int j) const;

    ~Shape();


signals:

public slots:

private:
    //定义 4 * 4的形状数组,6种形状，4种状态，4 * 4的数组
    static int SHAPES[6][4][4][4];

    int m_x, m_y;

    //一种形状的四种形式
    int m_currentShape[4][4][4];
    //当前形状的索引
    int m_shapeIndex;
    //当前的形状
    int m_shape[4][4];

    //父类的长和高
    int m_width, m_height;

    BackPanel *m_backPanel;

    bool m_canMoveDown;     //能否往下移动

};

#endif // SHAPE_H
